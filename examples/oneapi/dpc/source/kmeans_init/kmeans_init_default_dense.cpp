/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <CL/sycl.hpp>
#include <iomanip>
#include <iostream>

#define ONEAPI_DAL_DATA_PARALLEL
#include "oneapi/dal/algo/kmeans_init.hpp"
#include "oneapi/dal/data/accessor.hpp"

#include "example_util/utils.hpp"

using namespace oneapi;

void run(sycl::queue &queue) {
    constexpr std::int64_t row_count     = 8;
    constexpr std::int64_t column_count  = 2;
    constexpr std::int64_t cluster_count = 2;

    const float x_train_host[] = { 1.0,  1.0,  2.0,  2.0,  1.0,  2.0,  2.0,  1.0,
                                   -1.0, -1.0, -1.0, -2.0, -2.0, -1.0, -2.0, -2.0 };
    const float x_test_host[]  = { 1.0, 1.0, 2.0, 2.0 };

    auto x_train = sycl::malloc_shared<float>(row_count * column_count, queue);
    queue.memcpy(x_train, x_train_host, sizeof(float) * row_count * column_count).wait();
    const auto x_train_table = dal::homogen_table{ queue, row_count, column_count, x_train_host };
    const auto x_test_table  = dal::homogen_table{ cluster_count, column_count, x_test_host };

    const auto kmeans_init_desc = dal::kmeans_init::descriptor<>().set_cluster_count(cluster_count);

    const auto result = dal::train(queue, kmeans_init_desc, x_train_table);

    std::cout << "Initial cetroids:" << std::endl << result.get_centroids() << std::endl;

    std::cout << "Ground truth:" << std::endl << x_test_table << std::endl;

    sycl::free(x_train, queue);
}

int main(int argc, char const *argv[]) {
    for (auto device : list_devices()) {
        std::cout << "Running on " << device.get_info<sycl::info::device::name>() << std::endl;
        auto queue = sycl::queue{ device };
        run(queue);
    }
    return 0;
}