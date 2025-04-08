#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace {

struct SkeletonPass : public PassInfoMixin<SkeletonPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        // Create function declarations for
        // void incr_and_print_count() and void decr_and_print_count()
        // and add them to the module
        FunctionType *FT = FunctionType::get(Type::getVoidTy(M.getContext()), false);
        FunctionCallee incr_and_print_count = M.getOrInsertFunction("incr_and_print_count", FT);
        FunctionCallee decr_and_print_count = M.getOrInsertFunction("decr_and_print_count", FT);

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            for (auto &BB : F) {
                errs() << "  Basic Block: " << BB.getName() << "\n";
                for (auto &I : BB) {
                    // Find all callinsts
                    if (auto *CI = dyn_cast<CallInst>(&I)) {
                        // Print the function name
                        if (Function *Callee = CI->getCalledFunction()) {
                            // if the called function is malloc
                            if (Callee->getName() == "malloc") {
                                IRBuilder<> IRB(&I);
                                IRB.CreateCall(incr_and_print_count);
                            } else if (Callee->getName() == "free") {
                                IRBuilder<> IRB(&I);
                                IRB.CreateCall(decr_and_print_count);
                            } 
                        }
                    }
                }
            }
        }
        return PreservedAnalyses::all();
    };
};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(SkeletonPass());
                });
        }
    };
}
