require_rv64;
require_fp;
softfloat_roundingMode = VFRM;
WRITE_FRD(ui64_to_f64(RS1));
set_fp_exceptions;
