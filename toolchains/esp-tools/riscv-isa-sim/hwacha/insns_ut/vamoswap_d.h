require_rv64;
if(VPRED){
reg_t v = MMU.load_uint64(RS1);
MMU.store_uint64(RS1, RS2);
WRITE_RD(v);
}
