/* (c) 2012 Álan Crístoffer */

var App = {
    databaseChanged: function() {
        if ( AppCore.checkDatabase() )
            AppNav.navigated('#/category/show/0');
        else
            AppNav.navigated('#/database/showdialog');
    }
};

$(document).ready(function() {
    AppUI.clean();
    AppUI.listAvailableTranslations();
    AppUI.translate();
    
    //$('#open-dialog-bg').show();
    //$('#open-dialog').show();
});
