#include <stdint.h>
#include <stdlib.h>

#include "LLDB.h"

#include <string>

using namespace lldb;

class ScopedLLDB {
public:
  ScopedLLDB() {
    SBDebugger::Initialize();
  }
  ~ScopedLLDB() {
    SBDebugger::Terminate();
  }
};

int main(int argc, char const *argv[]) {
  ScopedLLDB sentry;

  SBDebugger debugger(SBDebugger::Create());

  // Create a debugger instance so we can create a target
  if (!debugger.IsValid()) {
    fprintf(stderr, "error: failed to create a debugger object\n");
    return 1;
  }

  const char* arch = "x86_64";
  const char* platform = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: cd <exe_name>\n");
    return 1;
  }

  const char *exe_file_path = argv[1];
  const bool add_dependent_libs = false;
  SBError error;
  SBStream strm;
  strm.RedirectToFileHandle(stdout, false);

  // Create a target using the executable.
  SBTarget target = debugger.CreateTarget(exe_file_path, arch, platform,
                                          add_dependent_libs, error);
  if (!error.Success()) {
    fprintf(stderr, "error: %s\n", error.GetCString());
    exit(1);
  }

  SBProcess process = target.LoadCore("core");

  printf("num threads: %d\n", process.GetNumThreads());
  for (int thread_i = 0; thread_i < process.GetNumThreads(); ++thread_i) {
    SBThread thread = process.GetThreadAtIndex(thread_i);
    int num_frames = thread.GetNumFrames();
    printf("  thread %d, num frames: %d\n", thread_i, num_frames);
    for (int i = 0; i < num_frames; ++i) {
      SBFrame frame = thread.GetFrameAtIndex(i);
      printf("    frame %d: %s\n", i, frame.GetDisplayFunctionName());
      SBValueList value_list = frame.GetVariables(true, true, true, true);
      for (int j = 0; j < value_list.GetSize(); ++j) {
        SBValue value = value_list.GetValueAtIndex(j);
        printf("      %s %s (@ %s) = %s",
               value.GetDisplayTypeName(),
               value.GetName(),
               value.GetLocation(),
               value.GetValue());
        const char* summary = value.GetSummary();
        if (summary)
          printf(" [%s]\n", summary);
        else
          printf("\n");
      }
    }
  }

  printf("process exit status: %d\n", process.GetExitStatus());

  return 0;
}
