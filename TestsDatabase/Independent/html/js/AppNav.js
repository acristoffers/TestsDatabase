/* (c) 2012 Álan Crístoffer */

var AppNav = {
    execute: function(cmd) {
        cmdlist = {
            database: {
                choose: AppCore.openDialogAndChooseFile,
                create: AppCore.openDialogAndCreateFile,
                showdialog: AppUI.showOpenDialog,
                close:  AppCore.closeDatabase
            },
            
            category: {
                show: AppUI.categoryShow
            }
        };
        if ( cmd.id )
            cmdlist[cmd.model][cmd.action]([cmd.id]);
        else
            cmdlist[cmd.model][cmd.action]();
    },
    
    navigated: function(hash) {
        var cmd = this.parseCommand(hash);
        if ( cmd.model ) this.execute(cmd);
    },
    
    parseCommand: function(hash) {
        var cmd = {};
        var split = hash.split('/');
        
        if ( split ) {
            if ( split[1] )
                cmd.model  = split[1];
            if ( split[2] )
                cmd.action = split[2];
            if ( split[3] )
                cmd.id     = split[3];
        }
        
        return cmd;
    }
};

window.onhashchange = function() {
    AppNav.navigated(location.hash);
};
