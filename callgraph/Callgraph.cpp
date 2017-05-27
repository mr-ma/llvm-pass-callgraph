#include "llvm/ADT/SCCIterator.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct CallgraphPass : public ModulePass {
  static char ID;
  CallgraphPass() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    CallGraphWrapperPass *CGPass =
        getAnalysisIfAvailable<CallGraphWrapperPass>();
    CallGraph *CG = CGPass ? &CGPass->getCallGraph() : nullptr;
    if (CG == nullptr) {
      errs() << "ERROR: No CallGraph\n";
      return false;
    }

    for (auto i = CG->begin(), ie = CG->end(); i != ie; i++) {
      const Function *func = (*i).first;

      if (func != nullptr && func->size() != 0) {
        errs() << "Function " << func->getName() << "\n";
      } else {
        errs() << "Function is null or size is 0\n";
      }

      CallGraphNode *node = (*i).second.get();
      Function *func2 = node->getFunction();

      if (func2 != nullptr) {
        errs() << "Function2 is not null ";
        errs() << "and size is: " << func2->size() << "\n"; // SegFault here!
        if (func2->size() != 0) {
          errs() << "Function2 " << func->getName() << "\n";
        }
      } else {
        errs() << "Function is null\n";
      }
    }

    return false;

    // Using SCC iterator
    // CallGraph &CG = getAnalysis<CallGraphWrapperPass>().getCallGraph();
    //
    // for (auto i = CG.begin(), ie = CG.end(); i != ie; i++) {
    //   const Function *func = (*i).first;
    //
    //   if (func != nullptr && func->size() != 0) {
    //     errs() << "Function " << func->getName() << "\n";
    //   } else {
    //     errs() << "Function is null or size is 0\n";
    //   }
    //   CallGraphNode *node = (*i).second.get();
    // 	Function *func2 = node->getFunction();
    //
    // 	if (func2 != nullptr) {
    // 		errs()<<"Function2 is not null ";
    // 		errs()<<"and size is: " << func2->size()  << "\n"; // SegFault
    // here!
    // 		if(func2->size() != 0) {
    // 			errs()<<"Function2 " << func->getName() << "\n";
    // 		}
    // 	} else {
    // 		errs()<<"Function is null\n";
    // 	}
    // }

    return false;
  }
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<CallGraphWrapperPass>();
    AU.setPreservesAll();
  }
};
}

char CallgraphPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerCallgraphPass(const PassManagerBuilder &,
                                  legacy::PassManagerBase &PM) {
  PM.add(new CallgraphPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerCallgraphPass);
static RegisterPass<CallgraphPass> X("callgraph", "runs call graph analysis");
