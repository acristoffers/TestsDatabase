/* (c) 2012 Álan Crístoffer */

var AppUI = {
    categoryShow: function(id) {
        AppUI.clean();
    },
    
    clean: function() {
        $('#open-dialog-bg').hide();
        $('#open-dialog').hide();
        $('#path-navigator').hide();
        $('#test-list-toolbar').hide();
        $('#category-list-btn-toolbar').hide();
        $('#categories-list-wrapper').hide();
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
    
    showOpenDialog: function() {
        AppUI.clean();
        $('#open-dialog-bg').show('fade');
        $('#open-dialog').show('fade');
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
