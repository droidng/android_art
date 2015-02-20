/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_OPTIMIZING_REFERENCE_TYPE_PROPAGATION_H_
#define ART_COMPILER_OPTIMIZING_REFERENCE_TYPE_PROPAGATION_H_

#include "driver/dex_compilation_unit.h"
#include "handle_scope-inl.h"
#include "nodes.h"
#include "optimization.h"

namespace art {

/**
 * Propagates reference types to instructions.
 */
class ReferenceTypePropagation : public HOptimization {
 public:
  ReferenceTypePropagation(HGraph* graph,
                           const DexFile& dex_file,
                           const DexCompilationUnit& dex_compilation_unit,
                           StackHandleScopeCollection* handles)
    : HOptimization(graph, true, "reference_type_propagation"),
      dex_file_(dex_file),
      dex_compilation_unit_(dex_compilation_unit),
      handles_(handles),
      worklist_(graph->GetArena(), kDefaultWorklistSize) {}

  void Run() OVERRIDE;

 private:
  void VisitNewInstance(HNewInstance* new_instance);
  void VisitLoadClass(HLoadClass* load_class);
  void VisitBasicBlock(HBasicBlock* block);
  void ProcessWorklist();
  void AddToWorklist(HPhi* phi);
  void AddDependentInstructionsToWorklist(HPhi* phi);
  bool UpdateNullability(HPhi* phi);
  bool UpdateReferenceTypeInfo(HPhi* phi);

  const DexFile& dex_file_;
  const DexCompilationUnit& dex_compilation_unit_;
  StackHandleScopeCollection* handles_;

  GrowableArray<HPhi*> worklist_;

  static constexpr size_t kDefaultWorklistSize = 8;

  DISALLOW_COPY_AND_ASSIGN(ReferenceTypePropagation);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_REFERENCE_TYPE_PROPAGATION_H_
