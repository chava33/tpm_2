cho ""
echo "Initialize Regression Test Keys"
echo ""

echo "------tpm2_listpcrs--------------------------"
./tpm2_listpcrs
echo "------tpm2_getrandom---------random.out------"
./tpm2_getrandom -s 20 -o random.out

echo "------tpm2_takeownership--------ops-eps-lps--"
./tpm2_takeownership -o pps -e eps -l lps 

echo "------tpm2_getpubek---------ekPasswd---------"
./tpm2_getpubek -e eps -o ops -P ekPasswd -H 0x81010001 -g 0x01 -f ek.pub



with nulll passwrd
bbox@bbox:~/Downloads/tpm2.0-tools/sapi-tools$ ./tpm2_createprimary -A o -g 0x0004 -G 0x0001
nameAlg = 0x0004
type = 0x0001
CreatePrimary Succeed ! Handle: 0x80000001

bbox@bbox:~/Downloads/tpm2.0-tools/sapi-tools$ ./tpm2_createprimary -A e -g 0x0004 -G 0x0001
nameAlg = 0x0004
type = 0x0001
CreatePrimary Succeed ! Handle: 0x80000000

bbox@bbox:~/Downloads/tpm2.0-tools/sapi-tools$ ./tpm2_createprimary -A o -g 0x000B -G 0x0001
nameAlg = 0x000b
type = 0x0001
CreatePrimary Succeed ! Handle: 0x80000001

bbox@bbox:~/Downloads/tpm2.0-tools/sapi-tools$ ./tpm2_createprimary -A o -g 0x000B -G 0x0023
nameAlg = 0x000b
type = 0x0023
CreatePrimary Failed ! ErrorCode: 0x2ca






try this
./tpm2_createprimary -A e -P eps -K def456 -g 0x000B -G 0x0008

./tpm2_readpublic -H 0x80000000 -opu test.txt  --not working

./tpm2_createprimary -A e -P eps -K def456 -g 0x000B -G 0x0008



lockout after this

bbox@bbox:~/Downloads/tpm2.0-tools/sapi-tools$ ./tpm2_createprimary -A p -g 0x000B -G 0x0008
nameAlg = 0x000b
type = 0x0008

CreatePrimary Failed ! ErrorCode: 0x9a2
