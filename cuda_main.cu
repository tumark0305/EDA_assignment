#include "cuda_include.cuh"
//max blocks is 1104
__device__ int2 overlap(const simple_BlockInfo& blocka, const simple_BlockInfo& blockb) {
    int x1_a = blocka.coordinate.x;
    int x2_a = x1_a + blocka.size.x;
    int y1_a = blocka.coordinate.y;
    int y2_a = y1_a + blocka.size.y;

    int x1_b = blockb.coordinate.x;
    int x2_b = x1_b + blockb.size.x;
    int y1_b = blockb.coordinate.y;
    int y2_b = y1_b + blockb.size.y;

    int x_overlap = max(0, min(x2_a, x2_b) - max(x1_a, x1_b));
    int y_overlap = max(0, min(y2_a, y2_b) - max(y1_a, y1_b));

    return make_int2(x_overlap, y_overlap);
}

__device__ int2 compute_overlap(const simple_BlockInfo& a, const simple_BlockInfo& b) {
    int dx = max(0, min(a.coordinate.x + a.size.x, b.coordinate.x + b.size.x) - max(a.coordinate.x, b.coordinate.x));
    int dy = max(0, min(a.coordinate.y + a.size.y, b.coordinate.y + b.size.y) - max(a.coordinate.y, b.coordinate.y));
    return make_int2(dx, dy);
}

__global__ void spring_method_kernel_shared(
    simple_BlockInfo* d_block_data,
    int2* d_sum_force,
    size_t block_count,
    float* d_rand_vals,
    size_t rand_count
) {
    //int a = blockIdx.y;
    //int b = blockIdx.x * blockDim.x + threadIdx.x;
    int a = blockIdx.x * blockDim.x + threadIdx.x;
    int b = blockIdx.y * blockDim.y + threadIdx.y;

    if (a >= block_count || b >= block_count || a >= b) return;

    simple_BlockInfo blockA = d_block_data[a];
    simple_BlockInfo blockB = d_block_data[b];
    int2 overlap_size = compute_overlap(blockA, blockB);
    if (overlap_size.x > 0 && overlap_size.y > 0) {
        int2 spring_force = make_int2((overlap_size.x +1) / 2, (overlap_size.y+1 ) / 2);
        int2 min_force = spring_force;

        int thread_id = a * block_count + b;
        float rand1 = d_rand_vals[thread_id % rand_count];
        float rand2 = d_rand_vals[(thread_id + 1) % rand_count];
        //cout << rand1 << endl;
        // 控制力的方向：主要方向（較大重疊）
        if (rand1 < 0.8f) {
            if (spring_force.x > spring_force.y) min_force.x = 0;
            else min_force.y = 0;
        }
        else {
            if (spring_force.x < spring_force.y) min_force.x = 0;
            else min_force.y = 0;
        }

        // 控制力加到哪個 block
        if (rand2 < 0.7f) {
            atomicAdd(&d_sum_force[a].x, min_force.x);
            atomicAdd(&d_sum_force[a].y, min_force.y);
        }
        else {
            atomicAdd(&d_sum_force[b].x, -min_force.x);
            atomicAdd(&d_sum_force[b].y, -min_force.y);
        }
        
    }
    
}


__global__ void overlap_check_kernel(simple_BlockInfo* blocks, bool* result, int count) {
    int a = blockIdx.x * blockDim.x + threadIdx.x;
    if (a >= count) return;

    for (int b = a + 1; b < count; ++b) {
        int x1_a = blocks[a].coordinate.x;
        int x2_a = x1_a + blocks[a].size.x;
        int y1_a = blocks[a].coordinate.y;
        int y2_a = y1_a + blocks[a].size.y;

        int x1_b = blocks[b].coordinate.x;
        int x2_b = x1_b + blocks[b].size.x;
        int y1_b = blocks[b].coordinate.y;
        int y2_b = y1_b + blocks[b].size.y;

        int x_overlap = max(0, min(x2_a, x2_b) - max(x1_a, x1_b));
        int y_overlap = max(0, min(y2_a, y2_b) - max(y1_a, y1_b));

        if (x_overlap > 0 && y_overlap > 0) {
            *result = false;
        }
    }
}

vector<array<int, 2>> spring_cuda(vector< BlockInfo>& block_list_input) {
    std::vector<simple_BlockInfo> simple_block_list; 
    for (const auto& block : block_list_input) {
        simple_BlockInfo cell;
        cell.coordinate = make_int2(block.coordinate[0], block.coordinate[1]);
        cell.size = make_int2(block.size[0], block.size[1]);
        simple_block_list.push_back(cell); 
    } 
    
    size_t block_count = simple_block_list.size();
    std::vector<int2> sum_force(block_count, { 0, 0 }); 

    std::vector<float> rand_vals(1024); // 建議用 curand 做
    for (auto& v : rand_vals) v = static_cast<float>(rand()) / RAND_MAX; 
    //for (auto& v : rand_vals) cout << v << endl;
    //cout << block_count << endl;

    float* d_rand_vals; 
    cudaMalloc(&d_rand_vals, rand_vals.size() * sizeof(float)); 
    cudaMemcpy(d_rand_vals, rand_vals.data(), rand_vals.size() * sizeof(float), cudaMemcpyHostToDevice); 

    size_t rand_count = rand_vals.size(); // 傳給 kernel

    // Allocate memory on device
    simple_BlockInfo* d_block_data_copy;
    int2* d_sum_force; 
    cudaMalloc((void**)&d_block_data_copy, block_count * sizeof(simple_BlockInfo));
    cudaMalloc((void**)&d_sum_force, block_count * sizeof(int2)); 

    // Copy data to device
    cudaMemcpy(d_block_data_copy, simple_block_list.data(), block_count * sizeof(simple_BlockInfo), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sum_force, sum_force.data(), block_count * sizeof(int2), cudaMemcpyHostToDevice); 

    // Set up block and grid sizes
    dim3 blockDim(32,32);
    dim3 gridDim((block_count + blockDim.x - 1) / blockDim.x, (block_count + blockDim.y - 1) / blockDim.y);

    // Launch the kernel
    spring_method_kernel_shared << <gridDim, blockDim >> > (
        d_block_data_copy,
        d_sum_force,
        block_count,
        d_rand_vals,
        rand_count
        );
    cudaDeviceSynchronize(); 
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));
    }
    // Copy the result back to host
    std::vector<int2> sum_force_device(block_count); 
    cudaMemcpy(sum_force_device.data(), d_sum_force, block_count * sizeof(int2), cudaMemcpyDeviceToHost); 

    // Free device memory
    cudaFree(d_block_data_copy);
    cudaFree(d_sum_force);
    cudaFree(d_rand_vals); 
    // Assign to output
    vector<array<int, 2>> output; 
    for (size_t i = 0; i < block_count; ++i) {
        output.push_back({ sum_force_device[i].x,sum_force_device[i] .y});
        //cout << "force=" << sum_force_device[i].x << "," << sum_force_device[i].y << endl;
        //cout << "force=" << simple_block_list[i].size.x << "," << simple_block_list[i].size.y << endl;
    }
    return output;
}





bool no_overlap_cuda(vector<BlockInfo>& block_list_input) {
    
    //GPU.prt();
    int count = block_list_input.size();
    if (count <= 1) return true;

    std::vector<simple_BlockInfo> simple_blocks;
    for (const auto& block : block_list_input) {
        simple_BlockInfo sb;
        sb.coordinate = make_int2(block.coordinate[0], block.coordinate[1]);
        sb.size = make_int2(block.size[0], block.size[1]);
        simple_blocks.push_back(sb);
    }

    // Allocate device memory
    simple_BlockInfo* d_blocks;
    bool* d_result;
    bool result = true;

    cudaMalloc(&d_blocks, count * sizeof(simple_BlockInfo));
    cudaMalloc(&d_result, sizeof(bool));
    cudaMemcpy(d_blocks, simple_blocks.data(), count * sizeof(simple_BlockInfo), cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, &result, sizeof(bool), cudaMemcpyHostToDevice);

    // Launch kernel
    int threads = 256;
    int blocks = (count + threads - 1) / threads;
    overlap_check_kernel << <blocks, threads >> > (d_blocks, d_result, count);
    cudaDeviceSynchronize();

    // Copy back
    cudaMemcpy(&result, d_result, sizeof(bool), cudaMemcpyDeviceToHost);

    // Cleanup
    cudaFree(d_blocks);
    cudaFree(d_result);

    return result;
}




