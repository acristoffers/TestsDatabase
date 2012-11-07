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
    categoryEdit: function() {
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
	        
        	if ( id == 0 ) {
        		cat = {id: 0, name: AppI18N.translate('Root')};
        	} else {
        		cat = AppCore.categorySelect(id);
        	}
            
        	var html = '<div>';
            html += '<label>';
            html += '<input type="radio" name="category-tree-radio" value="' + cat.id + '">' + cat.name;
            html += '</label>';
	        
        	var children = AppCore.listCategories(id);
        	for (var i = 0; i < children.length; i++ ) {
                html += recurse_categories(children[i].id);
        	}
	        
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
            
            for (var i = categories.length - 1; i >= 0; i--){
                html += '<li><a href="#/category/show/' + categories[i]['id'] + '">' + categories[i]['name'] + '</a></li>';
            }
        }
        
        var questions = AppCore.listQuestions(id);
        
        if (questions && questions.length > 0) {
            html += '<li class="nav-header" data-i18n="Questions"></li>';
            
            sortArrayByObjectKey(questions, 'title');
            
            for (var i = questions.length - 1; i >= 0; i--){
                html += '<li><a href="#/question/show/' + questions[i]['id'] + '">' + questions[i]['title'] + '</a></li>';
            }
        }
        
        if ( id == AppNav.current.category ) {
            $("#categories-list").html(html);
        } else {
            if ( id == 0 ) {
                $("#categories-list").parent().hide("slide",{ direction: "right" },200, function() {
                    $("#categories-list").html(html);
                    AppUI.translate();
                }).show("slide", {},200);
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
                    $("#categories-list").parent().hide("slide",{ direction: "right" },200, function() {
                        $("#categories-list").html(html);
                        AppUI.translate();
                    }).show("slide", {},200);
                } else {
                    $("#categories-list").parent().hide("slide",{},200, function() {
                        $("#categories-list").html(html);
                        AppUI.translate();
                    }).show("slide", { direction: "right" },200);
                }
            }
        }
        
        AppUI.showBreadCrumb(id);
        
        AppNav.current.category = id;
        
        AppUI.translate();
    },
    
    clean: function() {
        $('#alert-modal').hide();
        
        $('#open-dialog-bg').hide();
        $('#open-dialog').hide();
        
        $('#path-navigator').hide();
        $('#test-list-toolbar').hide();
        $('#category-list-btn-toolbar').hide();
        
        $('#category-edit-wrapper').hide();
        $('#categories-list-wrapper').hide();
    },
    
    closeModals: function() {
        $(".reveal-modal").trigger("reveal:close");
        $('#open-dialog-bg').hide('fade');
        $('#alert-modal').hide('fade');
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
        
        $("#language-dropdown").html(html);
        
    	var elements = $("*").filter(function() {
    		return $(this).data("lang") != undefined; 
    	});
        
        $(elements).click(function() {
            AppI18N.currentLanguage = ($(this).data('lang'));
            AppUI.translate();
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
            sep = "<span class='divider'>/</span>";
            cat = AppCore.categorySelect(cat.parent);
        }
        categories.push('<li><a href="#/category/show/0" data-i18n="Root"></a><span class="divider">/</span></li>');
        categories.reverse();
        
		var html = "";
		for (var i = 0; i < categories.length; i++) {
			html += categories[i];
		}
		$("#path-navigator ul").html(html);
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
        
        $('#alert-modal-btn-primary').unbind();
        $('#alert-modal-btn-danger').unbind();
        
        $('#alert-modal-btn-primary').click(callback);
        $('#alert-modal-btn-danger').click(callback);
        
        AppUI.translate();
        
        $('#open-dialog-bg').show('fade');
        $('#alert-modal').show('fade');
    },
    
    translate: function() {
        var elements = $("*").filter(function() {
            return $(this).data("i18n") != undefined; 
        });
	    
        for( var i   = 0; i < elements.length; i++) {
            var e    = elements[i];
            
            var text = $(e).data('i18n');
            var numbers = null;
            
            if ( $(e).data('i18n-numbers') ) {
                numbers = $(e).data('i18n-numbers').toString().split(';');
            }
            
            if ( $(e).html ) {
                $(e).html(AppI18N.translate(text, numbers));
            } else if ( $(e).val ) {
                $(e).val(AppI18N.translate(text, numbers));
            }
        }
        
    	// begin add-icons
        var elements = $("*").filter(function() {
    		return $(this).data("icon") != undefined; 
    	});
    	for( var i = 0; i < elements.length; i++) {
            var e = elements[i];
        
            $(e).html( '<i class="icon-' + $(e).data('icon') + '"></i> ' + $(e).html());
        }
    }
};
