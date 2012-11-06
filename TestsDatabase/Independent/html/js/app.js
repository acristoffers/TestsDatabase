/* (c) 2012 Álan Crístoffer */

var App = {
    categoryDelete: function() {
        var id = AppNav.current.category;
        var parent = AppCore.categorySelect(AppNav.current.category).parent;
        
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'This operation can\'t be undone. Are you sure you want to delete this category?',
            button: {
                class: 'danger',
                text: 'Delete'
            }
        }, function() {
            function recursive_delete(id){
                var subcats = AppCore.listCategories(id);
                var subqstn = AppCore.listQuestions(id);
                for (var i = subqstn.length - 1; i >= 0; i--){
                    AppCore.questionDelete(subqstn[i].id);
                }
                for (var i = subcats.length - 1; i >= 0; i--){
                    recursive_delete(subcats[i].id);
                }
                AppCore.categoryDelete(id);
            }
            
            recursive_delete(id);
            
            AppNav.navigated('#/category/show/' + parent);
        });
    },
    
    categoryInsert: function() {
        var name = $('#category-new-modal-title').val();
        $('#category-new-modal-title').val('');
        
        AppCore.categoryInsert(name, AppNav.current.category);
        
        AppUI.closeModals();
        
        AppNav.navigated('#/category/show/' + AppNav.current.category);
    },
    
    categoryMerge: function() {
        var id = AppNav.current.category;
        
        var np = $("input[name=category-tree-radio]:checked").val();
        
        var subcats = AppCore.listCategories(id);
        for (var i = subcats.length - 1; i >= 0; i--) {
            var cat = subcats[i];
            AppCore.categoryUpdate(cat.id, cat.name, np);
        }
        
        var subqtns = AppCore.listQuestions(id);
        for (var i = subqtns.length - 1; i >= 0; i--){
            var q = subqtns[i];
            AppCore.questionUpdate(q.id, q.title, q.reference, q.difficulty, q.body, np);
        }
        
        AppCore.categoryDelete(id);
        
        AppNav.navigated('#/category/show/' + np);
    },
    
    categoryUpdate: function() {
        var id = AppNav.current.category;
        var name = $('#category-edit-form-name').val();
        var parent = $("input[name=category-tree-radio]:checked").val();
        
        AppCore.categoryUpdate(id, name, parent);
        
        AppNav.navigated('#/category/show/'+id);
    },
    
    databaseChanged: function() {
        if ( AppCore.checkDatabase() )
            AppNav.navigated('#/category/show/0');
        else
            AppNav.navigated('#/database/showdialog');
    },
    
    modelChanged: function() {
        App.databaseChanged();
    }
};

$(document).ready(function() {
    AppUI.clean();
    AppUI.listAvailableTranslations();
    AppUI.translate();
    
    $('#open-dialog-bg').show();
    $('#open-dialog').show();
});
