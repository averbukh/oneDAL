/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
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

#include "oneapi/dal/algo/subgraph_isomorphism/detail/select_kernel.hpp"
#include "oneapi/dal/algo/subgraph_isomorphism/backend/cpu/graph_matching_default_kernel.hpp"
#include "oneapi/dal/detail/policy.hpp"
#include "oneapi/dal/backend/dispatcher.hpp"

namespace oneapi::dal::preview::subgraph_isomorphism::detail {

template <typename Float, typename Method>
graph_matching_result backend_default<dal::detail::host_policy,
                                      Float,
                                      Method,
                                      dal::preview::detail::topology<std::int32_t>>::
operator()(const dal::detail::host_policy &policy,
           const descriptor_base &desc,
           const dal::preview::detail::topology<std::int32_t> &t_data,
           const dal::preview::detail::topology<std::int32_t> &p_data) {
    return dal::backend::dispatch_by_cpu(dal::backend::context_cpu{ policy }, [&](auto cpu) {
        return call_subgraph_isomorphism_default_kernel_int32<decltype(cpu)>(desc, t_data, p_data);
    });
}

template struct ONEDAL_EXPORT backend_default<dal::detail::host_policy,
                                              float,
                                              dal::preview::subgraph_isomorphism::method::fast,
                                              dal::preview::detail::topology<std::int32_t>>;

} // namespace oneapi::dal::preview::subgraph_isomorphism::detail
