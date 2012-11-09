/* (c) 2012 Álan Crístoffer */

var App = {
    categoryDelete: function() {
        var id = AppNav.current.category;
        var parent = AppCore.categorySelect(AppNav.current.category).parent;
        
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'Are you sure you want to delete this category?\nThis operation can\'t be undone.',
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
    
    questionDelete: function() {
        var id = AppNav.current.question;
        
        AppUI.showModalAlert({
            title: 'Are you sure?',
            content: 'Are you sure you want to delete this question?\nThis operation can\'t be undone.',
            button: {
                class: 'danger',
                text: 'Delete'
            }
        }, function() {
            AppCore.questionDelete(id);
            AppNav.navigated('#/category/show/' + AppNav.current.category);
        });
    },
    
    questionInsert: function() {
        AppNav.blank();
        
		var id = AppNav.current.question;
		var title = $("#question-form-title").val();
		var difficulty = $('#question-form-difficulty').slider('value');
		var reference = $("#question-form-reference").val();
		var body = $("#question-form-body").val();
		var category = $('input[name=category-tree-radio]:checked').val();
        
        if ( title == '' ) {
            AppUI.showModalAlert({
                title: 'No title',
                content: 'You forgot to type the question title...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
            return;
        }
		
		var answers = [];
		var right_answer = null;
		
		$('input[name^=question-form-answers-fields]').each(function() {
			var r = $(this).prev('input').is(':checked');
			if ( r ) {
				right_answer = $(this).val();
			} else {
				answers.push($(this).val());
			}
		});
		
		if ( right_answer == null ) {
            AppUI.showModalAlert({
                title: 'No right answer',
                content: 'You forgot to select the right answer...',
                cancel: false,
                button: {
                    text: 'Ok'
                }
            }, AppUI.closeModals);
			return;
		}
		
		if ( id < 0 ) {
			id = AppCore.questionInsert(title, reference, difficulty, body, category);
		} else {
			AppCore.questionUpdate(id, title, reference, difficulty, body, category);
			AppCore.answerDelete(id);
		}
        AppCore.answerInsert(right_answer, id, true);
		for (var i = 0; i < answers.length; i++ ) {
			AppCore.answerInsert(answers[i], id, false);
		}
		
		AppNav.current.category = category;
        AppNav.navigated('#/question/show/'+id);
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
