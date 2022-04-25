
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

extern SEXP resvg_();

static const R_CallMethodDef CEntries[] = {

  {"resvg_", (DL_FUNC) &resvg_, 2},
  {NULL , NULL, 0}
};


void R_init_resvgr(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



