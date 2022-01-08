function hide(variables){
    var container = document.querySelector('.grid');
    var msnry = new Masonry( container, {
        columnWidth: '.grid-sizer'
        });
    
    for (var i = 0; i < variables.length;i++){
        var name = "." + variables[i]
        $(name).hide();
 		msnry.layout();
    }
}

function show(variables){
    var container = document.querySelector('.grid');
    var msnry = new Masonry( container, {
        columnWidth: '.grid-sizer'
        });
    
    for (var i = 0; i < variables.length;i++){
        var name = "." + variables[i]
        $(name).show();
 		msnry.layout();
    }
}