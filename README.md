# encryptor
simple CLI encryption tool
assignment completed during cryptography class
# special features
uses intel SSE intrinsics to achieve maximum performance

```
git clone https://github.com/kistanod/encryptor
sh install.sh
```
examples
```
encryptor e superpassword < secret.txt > secret.encrypted
encryptor d superpassword < secret.encrypted > decrypted.txt
```
