require_fp;
softfloat_roundingMode = VFRM;
WRITE_FRD(f64_mulAdd(f32_to_f64(f32(f32(FRS1).v ^ F32_SIGN)), f32_to_f64(f32(FRS2)), f32_to_f64(f32(FRS3))));
set_fp_exceptions;
