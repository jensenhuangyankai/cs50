async function hide(variables){
    for (var i = 0; i < variables.length;i++){
        console.log(variables[i]);
        var instances = document.getElementsByClassName(variables[i]);

        for (var j = 0; j < instances.length; j ++) {
            instances[j].style.setProperty('-webkit-transition', 'opacity 300ms, visibility 300ms');
            instances[j].style.setProperty('transition','opacity 300ms, visibility 300ms');
            //instances[j].style.setProperty('position' , 'absolute');
            //instances[j].style.setProperty('list-style', 'none');
            instances[j].style.setProperty('opacity', '0');
            instances[j].style.setProperty('visibility', 'hidden');
            //instances[j].style.setProperty('padding', '10px');
            //instances[j].style.setProperty('background-color', 'rgba(92, 91, 87, 0.9)');
            
            //instances[j].style.setProperty('-webkit-transform-style', 'preserve-3d');
            //instances[j].style.setProperty('transform-style', 'preserve-3d');
            
            
        }
        await new Promise(r => setTimeout(r, 200));
        for (var j = 0; j < instances.length; j ++) {
            instances[j].style.setProperty('display' , 'none');
        }
    }
}

async function show(variables){
    for (var i = 0; i < variables.length;i++){
        console.log(variables[i]);
        var instances = document.getElementsByClassName(variables[i]);

        for (var j = 0; j < instances.length; j ++) {
            instances[j].style.setProperty('-webkit-transition', 'opacity 600ms, visibility 600ms');
            instances[j].style.setProperty('transition','opacity 600ms, visibility 600ms');

            instances[j].style.setProperty('visibility', 'visible');
            instances[j].style.setProperty('opacity', '1');
            
        }
        await new Promise(r => setTimeout(r, 50));
        for (var j = 0; j < instances.length; j ++) {
            instances[j].style.setProperty('display' , 'inline-flex');
        }

    }
}
