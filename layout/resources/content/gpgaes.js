
function GPGAESEncrypt(key, plaintext)
{
    var result = "";
    var component = componentmgr.getComponent("79463AD7-02C3-42FD-A8F0-061525C45FF8");
    if (component) {
        var anobject = component.getObject("6F2AD801-85A4-4FF4-A0F3-42B013ED2C8E");
        if (anobject) {
            try {
                dump("GPGAESEncrypt got the object\n");
                anobject.SetKey(key); 
                result = anobject.Encrypt(plaintext);
            } catch (ex) {
                dump(ex);
            }
            component.releaseObject(anobject);
        }
   }
   return result;
}

function HandleEncrypt()
{
    var key;
    var plaintext;

    plaintext = document.getElementById("plaintext").value;
    key = document.getElementById("key").value;

    if (key.length != 17) {
        document.getElementById("key").value = 
            "Key must be 16 characters long";
        return;
    }

    document.getElementById("ciphertext").value = 
        GPGAESEncrypt(key, plaintext);
}

