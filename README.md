# encryptor
simple CLI encryption tool  
idea comes from assignment completed in cryptography class
# special features
uses intel SSE intrinsics to achieve maximum performance
# to install run
```
git clone https://github.com/kistanod/encryptor
cd encryptor
sh install.sh
```
# examples
```
encryptor e superpassword secret.txt secret.encrypted
encryptor d superpassword secret.encrypted decrypted.txt
```
