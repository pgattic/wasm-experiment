#include <nds.h>
#include <ulib/ulib.h>
#include <fat.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <fstream>

#include <wasm3.h>
#include <m3_env.h>
#include "../rust/app.wasm.h"
#include "nds.h"

int main(void) {
  // Graphics Library Initialization
  ulInit(UL_INIT_ALL);
  ulInitGfx();
  ulInitText();

  IM3Environment env = m3_NewEnvironment ();
  IM3Runtime runtime = m3_NewRuntime (env, 1024, NULL);
  IM3Module module;
  m3_ParseModule (env, &module, target_wasm32_unknown_unknown_release_nds_rust_wasm, target_wasm32_unknown_unknown_release_nds_rust_wasm_len);
  m3_LoadModule (runtime, module);
  LinkNDSFunctions(module);
  IM3Function f;
  m3_FindFunction (&f, runtime, "start");
  m3_CallV (f, 10);
  return 0;
}

