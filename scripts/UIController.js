var pages = ['main', 'solidColor', 'audio', 'fadeAndFlash'];
var currentPage = 'main';

function openNav() {
    document.getElementById("UISidenav").style.width = "250px";
    document.getElementById("menuBar").style.marginLeft = "250px";
    for (i in pages) {
        document.getElementById(pages[i]).style.marginLeft = "250px";
    }
}

function closeNav() {
    document.getElementById("UISidenav").style.width = "0";
    document.getElementById("menuBar").style.marginLeft= "0";
    for (i in pages) {
        document.getElementById(pages[i]).style.marginLeft= "0";
    }
}

function showPage(pageName) {
    console.log("INFO: current page: " + currentPage + ", new page:" + pageName);
    document.getElementById(currentPage).style.display = 'none';
    document.getElementById(pageName).style.display = 'block';
    currentPage = pageName;
    closeNav();
}

function printColorValue(color) {
    console.log(color);
    console.log(typeof(color));
}
