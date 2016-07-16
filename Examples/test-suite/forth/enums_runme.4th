voctable enums
also enums definitions
c-library enums
include enums.4th
end-c-library

\ throw an error if constants mismatch
: =? ( n1 n2 -- ) <> abort" Constant does not match!" ;

CSP_ITERATION_FWD 0 =?
CSP_ITERATION_BWD 11 =?
ABCDE 0 =?
FGHJI 1 =?
boo 0 =?
hoo 5 =?
globalinstance1 0 =?
globalinstance2 1 =?
globalinstance3 30 =?
AnonEnum1 0 =?
AnonEnum2 100 =?

." Run Enums successfull" cr bye
