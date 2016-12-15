
// encrypt plaintext using key (in range [0, 25])

function CaesarEncrypt(key, plaintext)
{
    var result = "";
    var keyval= parseInt(key);
    plaintext = plaintext.toUpperCase();

   var length = plaintext.length;
   for (var i = 0; i < length; i++) {
       var val = plaintext.charCodeAt(i) - 65;
       if (val > 0)
            result += String.fromCharCode(((val + keyval) % 26) + 65);
   }
   return result;
}

function HandleEncrypt()
{
    var key;
    var plaintext;

    plaintext = document.getElementById("plaintext").value;
    key = document.getElementById("key").value;
    
    var ciphertext = CaesarEncrypt(key, plaintext);

    document.getElementById("ciphertext").value = ciphertext;    
}

