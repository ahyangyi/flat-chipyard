require_fp;
softfloat_roundingMode = VFRM;
WRITE_FRD(i32_to_f64((int32_t)RS1));
set_fp_exceptions;
