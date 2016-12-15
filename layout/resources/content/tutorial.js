function ClickHereHandler()
{
    var obj = document.getElementById("mytext");
    if (obj) {
        var value = obj.value;
        document.openDialog("resources/content/tutorial2.xul");
    } 
}
