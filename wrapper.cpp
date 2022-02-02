#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileUtilities.h"

#include "lld/Common/Driver.h"

#include <iostream>

#if OLD_LINK
bool lldInvoke(const char *inPath, const char *outPath) {
    return lld::elf::link({"ld.lld", "-shared", inPath,
                         "-o", outPath}, /*exitEarly=*/true,
                        llvm::outs(), llvm::errs());
}
#else
bool lldInvoke(const char *inPath, const char *outPath) {
    return lld::elf::link({"ld.lld", "-shared", inPath,
                         "-o", outPath},
                        llvm::outs(), llvm::errs(), /*exitEarly=*/true,
                        /*disableOutput=*/false);
}
#endif

enum class Status : bool { Success=0, Failure=1 };

Status runLinker(const char *path) {
    // Create a temp file for HSA code object.
    int tempHsacoFD = -1;
    llvm::SmallString<128> tempHsacoFilename;
    if (llvm::sys::fs::createTemporaryFile("kernel", "hsaco", tempHsacoFD,
                                             tempHsacoFilename)) {
        return Status::Failure;
    }
    llvm::FileRemover cleanupHsaco(tempHsacoFilename);
    // Invoke lld. Expect a true return value from lld.
    if (!lldInvoke(path, tempHsacoFilename.c_str())) {
        llvm::errs() << "Failed to link: " << path << "\n";
        return Status::Failure;
    }
    return Status::Success;
}

int main(int argc, const char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (runLinker(argv[i]) == Status::Failure) {
            return 1;
        }
    }
    return 0;
}
