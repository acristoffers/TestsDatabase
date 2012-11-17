/* (c) 2012 Álan Crístoffer */

var AppNav = {
    current: {
        question: 0,
        category: 0,
        test:     0
    },
    
    blank: function() {
        location.hash = '#blank';
    },
    
    execute: function(cmd) {
        cmdlist = {
            about: {
                show: AppUI.about
            },
            
            database: {
                choose:     AppCore.OpenFileDialog,
                create:     AppCore.SaveFileDialog,
                dummy:      App.populateDatabase,
                export:     AppCore.export,
                showdialog: AppUI.showOpenDialog,
                close:      AppCore.closeDatabase
            },
            
            category: {
                delete: App.categoryDelete,
                edit:   AppUI.categoryEdit,
                insert: App.categoryInsert,
                merge:  App.categoryMerge,
                show:   AppUI.categoryShow,
                update: App.categoryUpdate
            },
            
            question: {
                delete: App.questionDelete,
                edit:   AppUI.questionEdit,
                insert: App.questionInsert,
                new:    AppUI.questionNew,
                show:   AppUI.questionShow
            },
            
            test: {
                delete: App.testDelete,
                insert: App.testInsert,
                list:   AppUI.testList,
                new:    AppUI.testNew,
                show:   AppUI.testShow,
                
                printTests:        App.printTests,
                printAnswers:      App.printAnswers,
                printAnswerSheets: App.printAnswerSheets
            },
            
            modal: {
                close:        AppUI.closeModals,
                new_category: AppUI.categoryNew
            }
        };
        
        cmdlist[cmd.model][cmd.action]([cmd.id]);
        AppNav.blank();
    },
    
    navigate: function(hash) {
        if ( hash == '#/blank' )
            return;
        
        var cmd = AppNav.parseCommand(hash);
        if ( cmd.model ) AppNav.execute(cmd);
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
    AppNav.navigate(location.hash);
};
