/* (c) 2012 Álan Crístoffer */

// Does not account for numbers, and I'm not caring
function sortArrayByObjectKey(array, key)
{
    array.sort(function(a, b){
        var obj1 = a[key].toLowerCase();
        var obj2 = b[key].toLowerCase();
        
        if ( obj1 < obj2 )
            return 1;
        
        if ( obj1 > obj2 )
            return -1;
            
        return 0;
    });
}

var AppUI = {
    about: function() {
        AppNav.blank();
        $('#about-dialog').reveal();
    },
    
    categoryEdit: function() {
        AppNav.navigate('#/category/show/' + AppNav.current.category);
        
        var id = AppNav.current.category;
        
        $('#category-edit-form-parent-tree').html(AppUI.categoryHTMLTree(id));
        $('#category-edit-form-parent-tree input[value=' + AppCore.categorySelect(id).parent + ']').click();
        
        $('#category-edit-wrapper').show('fade');
        
        $('#category-edit-form-name').val(AppCore.categorySelect(id).name);
        $('#category-edit-form-name').focus();
    },
    
    categoryHTMLTree: function(skip) {
        function recurse_categories(id) {
        	var cat;
            
            if ( id == skip )
                return '';
	        
        	if ( id == 0 )
        		cat = {id: 0, name: AppI18N.tr('Root')};
            else
        		cat = AppCore.categorySelect(id);
            
        	var html = '<div>';
            html += '<label>';
            html += '<input type="radio" name="category-tree-radio" value="' + cat.id + '">' + cat.name;
            html += '</label>';
	        
        	var children = AppCore.listCategories(id);
        	for (var i = 0; i < children.length; i++ )
                html += recurse_categories(children[i].id);
	        
        	html += '</div>';
        	return html;
        }
        
        return recurse_categories(0);
    },
    
    categoryNew: function() {
        $('#category-new-modal').reveal();
        $('#category-new-modal-title').focus();
    },
    
    categoryShow: function(id) {
        AppUI.clean();
        $('#category-list-btn-toolbar').show();
        $('#categories-list-wrapper').show();
        
        if (id == undefined)
            id = 0;
            
        if ( id == 0 )
            $('#category-list-edit').hide();
        else
            $('#category-list-edit').show();
        
        var categories = AppCore.listCategories(id);
        
        var html = '';
        
        if (categories && categories.length > 0) {
            html = '<li class="nav-header" data-i18n="Categories"></li>';
            
            sortArrayByObjectKey(categories, 'name');
            
            for (var i = categories.length - 1; i >= 0; i--)
                html += '<li><a href="#/category/show/' + categories[i]['id'] + '">' + categories[i]['name'] + '</a></li>';
        }
        
        var questions = AppCore.listQuestions(id);
        
        if (questions && questions.length > 0) {
            html += '<li class="nav-header" data-i18n="Questions"></li>';
            
            sortArrayByObjectKey(questions, 'title');
            
            for (var i = questions.length - 1; i >= 0; i--)
                html += '<li><a href="#/question/show/' + questions[i]['id'] + '">' + questions[i]['title'] + '</a></li>';
        }
        
        if ( parseInt(id) == parseInt(AppNav.current.category) ) {
            $('#categories-list').html(html);
        } else {
            if ( id == 0 ) {
                $('#categories-list').parent().hide('slide',{ direction: 'right' },200, function() {
                    $('#categories-list').html(html);
                    AppI18N.translate();
                }).show('slide', {},200);
            } else {
                var eltern = [];
                if ( AppNav.current.category != 0 ) {
                    var cat = AppCore.categorySelect(AppNav.current.category);
                    eltern.push(parseInt(cat.id));
                    while( cat.parent != 0 ) {
                        eltern.push(parseInt(cat.parent));
                        cat = AppCore.categorySelect(cat.parent);
                    }
                }

                if ( AppCore.ArrayHaveElement(eltern, id) ) {
                    $('#categories-list').parent().hide('slide',{ direction: 'right' },200, function() {
                        $('#categories-list').html(html);
                        AppI18N.translate();
                    }).show('slide', {},200);
                } else {
                    $('#categories-list').parent().hide('slide',{},200, function() {
                        $('#categories-list').html(html);
                        AppI18N.translate();
                    }).show('slide', { direction: 'right' },200);
                }
            }
        }
        
        AppUI.showBreadCrumb(id);
        
        AppNav.current.category = id;
        
        AppI18N.translate();
    },
    
    clean: function() {
        $('#alert-modal').hide();
        
        $('#open-dialog-bg').hide();
        $('#open-dialog').hide();
        
        $('#path-navigator').hide();
        $('#categories-list-wrapper').hide();
        $('#test-list-toolbar').hide();
        $('#category-list-btn-toolbar').hide();
        
        // category edit
        $('#category-edit-wrapper').hide();
        $('#question-form-answers').empty();
        $('#category-edit-form-parent-tree').empty();
        
        // question form
        $('#question-form-category-tree').empty();
        $('#question-form-answers').empty();
        $('#question-form-body').val('');
        $('#question-form-wrapper').hide();
        
        // question show
        $('#question-show').hide();
        $('#question-show-answers').empty();
        $('#question-show-body').empty();
        
        // test form
        $('#test-create').hide();
        $('#test-create-questions').empty();
        $('#test-form').hide();
        $('#test-form-categories').empty();
        
        // test show
        $('#test-show').hide();
        $('#test-show-body').empty();
        
        // answer sheet print
        $('#answers-sheet').hide();
        $('#answers-header').empty();
        $('#answers').empty();
    },
    
    closeModals: function() {
        $('.reveal-modal').trigger('reveal:close');
        $('#open-dialog-bg').hide('fade');
        $('#alert-modal').hide('fade');
    },
    
    questionEdit: function() {
        AppNav.navigate('#/category/show/' + AppNav.current.category);
        
        var q = AppCore.questionSelect(AppNav.current.question);
        
		$('#question-form-title').val(q.title);
		$('#question-form-difficulty').slider('value', q.difficulty);
		$('#question-form-reference').val(q.reference);
		$('#question-form-body').val(q.body);
		
		var as = AppCore.answerSelect(AppNav.current.question);
        var anw = '';
		for (var i = 0; i < as.length; i++)
			anw += '<div class="input-prepend"><input class="add-on" type="radio" name="question-form-rigth-answer" ' + (parseInt(as[i].right)?'checked':'') + '><input type="text" name="question-form-answers-fields[]" class="input-xxlarge" value="' + as[i].body + '""></div>';
        
        $('#question-form-answers').html(anw);
        
        $('#question-form-category-tree').html(AppUI.categoryHTMLTree());
        $('#question-form-category-tree input[value=' + q.category + ']').click();
        
        $('#question-add-span').hide();
        $('#question-edit-span').show();
        $('#question-form-wrapper').show('fade');
        
    	$('#question-form-body').cleditor()[0].refresh();
    	$('#question-form-body').cleditor()[0].updateFrame();
        
        $('#question-form-title').focus();
    },
    
    questionNew: function() {
        AppNav.navigate('#/category/show/' + AppNav.current.category);
        
        AppNav.current.question = -1;
        
		$('#question-form-title').val('');
		$('#question-form-difficulty').slider('value', 6);
		$('#question-form-reference').val('');
        
        $('#question-form-category-tree').html(AppUI.categoryHTMLTree());
        $('#question-form-category-tree input[value=' + AppNav.current.category + ']').click();
        
        var anw = '';
        for(var i=0; i<4; i++)
            anw += '<div class="input-prepend"><input class="add-on" type="radio" name="question-form-rigth-answer"><input type="text" name="question-form-answers-fields[]" class="input-xxlarge"></div>';
        
        $('#question-form-answers').html(anw);
        
        $('#question-add-span').show();
        $('#question-edit-span').hide();
        $('#question-form-wrapper').show('fade');
        
    	$('#question-form-body').cleditor()[0].refresh();
    	$('#question-form-body').cleditor()[0].updateFrame();
        
        $('#question-form-title').focus();
    },
    
    questionShow: function(id) {
        AppNav.navigate('#/category/show/' + AppNav.current.category);
        AppNav.current.question = id;
        
        var q = AppCore.questionSelect(id);
	    
    	$('#question-show-title').html(q.title);
    	$('#question-show-reference').html ('<b data-i18n="Reference:"></b> ' + q.reference);
    	$('#question-show-difficulty').html('<b data-i18n="Difficulty:"></b> ' + q.difficulty);
    	$('#question-show-body').html(q.body);
	
    	var as = AppCore.answerSelect(id);
    	var html = '';
    	for (var i = 0; i < as.length; i++) {
    		html += '<li>' + (parseInt(as[i].right)?'<b>':'') + as[i].body + (parseInt(as[i].right)?'</b>':'') + '</li>';
    	}
    	$('#question-show-answers').html(html);
        
        AppI18N.translate();
        $('#question-show').show('fade');
    },
    
    listAvailableTranslations: function() {
        var langs = AppI18N.availableLanguages();
        
        var ordered_langs = [];
        
        for (key in langs) {
            ordered_langs.push(key);
        };
        
        ordered_langs.sort();
        
        var html = '';
        
        for (var i=0; i < ordered_langs.length; i++) {
            var key = ordered_langs[i];
            html += '<li><a data-lang="' + langs[key] + '">' + key + '</a></li>';
        }
        
        $('#language-dropdown').html(html);
        
    	var elements = $('*').filter(function() {
    		return $(this).data('lang') != undefined; 
    	});
        
        $(elements).click(function() {
            AppI18N.currentLanguage = ($(this).data('lang'));
            AppI18N.translate();
        });
    },
    
    showBreadCrumb: function(id) {
        if ( id == 0 ) {
            $('#path-navigator').hide();
            return;
        }
        
        $('#path-navigator').show();
        
        var cat = AppCore.categorySelect(id);
        
        var categories = [];
        var sep = '';
        while(cat.id > 0) {
            categories.push('<li><a href="#/category/show/' + cat.id + '">' + cat.name + '</a>' + sep + '</li>');
            sep = '<span class="divider">/</span>';
            cat = AppCore.categorySelect(cat.parent);
        }
        categories.push('<li><a href="#/category/show/0" data-i18n="Root"></a><span class="divider">/</span></li>');
        categories.reverse();
        
		var html = '';
		for (var i = 0; i < categories.length; i++) {
			html += categories[i];
		}
		$('#path-navigator ul').html(html);
    },
    
    showOpenDialog: function() {
        AppUI.clean();
        $('#open-dialog-bg').show('fade');
        $('#open-dialog').show('fade');
    },
    
    showModalAlert: function(conf, callback) {
        $('#alert-modal-title h1').data('i18n', conf.title);
        $('#alert-modal-content').data('i18n' ,conf.content);
        
        if ( conf.button.class == 'danger' ) {
            $('#alert-modal-btn-primary').hide();
            $('#alert-modal-btn-danger').show();
            $('#alert-modal-btn-danger').data('i18n', conf.button.text);
        } else {
            $('#alert-modal-btn-danger').hide();
            $('#alert-modal-btn-primary').show();
            $('#alert-modal-btn-primary').data('i18n', conf.button.text);
        }
        
        if ( conf.cancel == false ) {
            $('#alert-modal-btn-cancel').hide();
        } else {
            $('#alert-modal-btn-cancel').show();
        }
        
        $('#alert-modal-btn-primary').unbind();
        $('#alert-modal-btn-danger').unbind();
        
        $('#alert-modal-btn-primary').click(callback);
        $('#alert-modal-btn-danger').click(callback);
        
        AppI18N.translate();
        
        $('#open-dialog-bg').show('fade');
        $('#alert-modal').show('fade');
    },
    
    testCreate: function() {
        AppNav.navigate('#/test/list');
        AppNav.blank();
        
        $('#test-create').show();
        
        $('#test-create-questions').html( AppUI.testCreateQuestionsTree() );
        
        $('#test-create-questions input').click(function() {
            var count = $('#test-create-questions input:checked').size();
            $('#test-create-total-questions').data('i18n-numbers', count);
            AppI18N.translate();
        });
        
        AppI18N.translate();
    },
    
    testCreateQuestionsTree: function() {
        function recurse_categories(id) {
        	var cat;
            	        
        	if ( id == 0 )
        		cat = {id: 0, name: 'Root'};
            else
        		cat = AppCore.categorySelect(id);
            
            var questions = AppCore.listQuestions(cat.id);
            
        	var html = '<div>';
            html += '<span data-i18n="' + cat.name + '"></span>'
            for (var i = questions.length - 1; i >= 0; i--) {
                var question = questions[i];
                
                html += '<label>';
                html += '<input type="checkbox" name="category-tree-check" value="' + question.id + '"> ' + question.title;
                html += '</label>';
            }
	        
        	var children = AppCore.listCategories(id);
        	for (var i = 0; i < children.length; i++ )
                html += recurse_categories(children[i].id);
	        
        	html += '</div>';
        	return html;
        }
        
        return recurse_categories(0);
    },
    
    testList: function() {
        AppUI.clean();
        $('#test-list-toolbar').show();
        $('#categories-list-wrapper').show();
        
        var html = '';
        
        var tests = AppCore.listTests();
        
        if ( tests && tests.length > 0 ) {
            html += '<li class="nav-header" data-i18n="Tests"></li>';
            
            sortArrayByObjectKey(tests, 'title');
            
            for (var i = tests.length - 1; i >= 0; i--)
                html += '<li><a href="#/test/show/' + tests[i]['id'] + '">' + tests[i]['title'] + '</a></li>';
        }
        
        $('#categories-list-wrapper ul').html(html);
        AppI18N.translate();
    },
    
    testCategories: function(id) {
        if ( id == undefined ) id = 0;
        
    	var cat;
	    
    	if ( id == 0 )
    		cat = {id:0, name:AppI18N.tr('Root')};
        else
    		cat = AppCore.categorySelect(id);
        
        var html = '<div>' + 
                   '<label class="checkbox" for="test-category-checkbox-' + cat.id + '">' +
                   '<input data-id="' + cat.id + '" id="test-category-checkbox-' + cat.id + '" type="checkbox"> ' + cat.name +
                   '<a data-icon="check" data-i18n="" class="btn btn-link select-all-subitems"></a>' +
                   '</label>';
        
        var children = AppCore.listCategories(id);
    	for (var i = 0; i < children.length; i++ )
            html += AppUI.testCategories(children[i].id);
        
        html += '</div>';
        
        return html;
    },
    
    testNew: function() {
        AppNav.navigate('#/test/list');
        AppNav.blank();
        
        $('#test-form').show('fade');
        
        $('#test-form-question-difficulties').empty();
        
        function addDiff() {
            function changeQTotal() {
                var i = 0;
                $('#test-form-question-difficulties .questions').each(function() {
                    i += parseInt($(this).val());
                });
                $('#test-form-total-questions').data('i18n-numbers', i);
                AppI18N.translate();
            }
            
            var h = '<div class="difficulty-setup"><span data-i18n="Number of questions"></span>' +
                    '<input class="questions" type="number" min="0" max="50" value="10">' +
                    '<span data-i18n="Difficulty from""></span>' +
                    '<input class="diff-from" type="number" min="0" max="9" value="6">' +
                    '<span data-i18n="to"></span>' +
                    '<input class="diff-to" type="number" min="1" max="10" value="7">' +
                    '<a class="btn test-form-diff-remove" data-i18n="Remove"></a></div>';
            $('#test-form-question-difficulties').append(h);
            
            $('.test-form-diff-remove').unbind().click(function() {
                $(this).parent().remove();
                changeQTotal();
            });
            
            $('#test-form-question-difficulties input[type=number]').unbind().change(changeQTotal);
            
            changeQTotal();
            
            AppI18N.translate();
        }
        
        addDiff();
        addDiff();
        
        $('#test-form-add-difficulty').unbind().click(addDiff);
        
        $('#test-form-diff-remove').unbind();
        
        $('#test-form-categories').html(AppUI.testCategories());
        $('#test-form-categories .select-all-subitems').click(function(e) {
            e.preventDefault();
            $(this).parent().parent().find('input').attr('checked',true);
        });
        
        $('#test-form-header').val(AppCore.testHeader());
    	$('#test-form-header').cleditor()[0].refresh();
    	$('#test-form-header').cleditor()[0].updateFrame();
        
        AppI18N.translate();
    },
    
    testShow: function(id) {
        AppNav.navigate('#/test/list');
        
        AppNav.current.test = id;
        
        if ( id == undefined )
            AppNav.navigate('#/category/show/0');
        
        var t = AppCore.testSelect(id);
        
        $('#test-show-title').html(t.title);
        $('#test-show-date').html(t.date);
        $('#test-show-body').html(t.body);
		
		$('#print-test-num').val( 1 );
		$('#print-test-num').attr('max', $('.test').length );
        
        $('#test-show').show('fade');
        AppI18N.translate();
    }
};

$(document).ready(function() {
	$('#question-form-difficulty').slider({animate:true, max:10, value:6});
	$('#question-form-difficulty' ).bind( 'slidechange', function(event, ui) {
		$('#question-form-difficulty-span').html(ui.value);
	});
    
    $('#test-form-difficulty').slider({animate:true, max:10, values:[0,6], range:true});
	
    $('textarea').cleditor({width: 600, height: 400});
});