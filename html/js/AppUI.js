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
        var langs = JSON.parse(AppCore.availableTranslations());
        langs['English'] = 'en';
        
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
            AppCore.installTranslation($(this).data('lang'));
            AppUI.translate();
        });
    },
    
    showOpenDialog: function() {
        AppUI.clean();
        $('#open-dialog-bg').show('fade');
        $('#open-dialog').show('fade');
    },
    
    translate: function() {
        /*
        The lines marked with "ease translation process"
        are used to ouput the content of the html 'section'
        of UIBridge::htmlTranslations() so I don't have to
        type all the tr() lines that are used by lupdate
        and thus can be removed if necessary (production, for example)
        */
        var elements = $("*").filter(function() {
            return $(this).data("i18n") != undefined; 
        });
    
        var trs      = []; // ease translation process
        var cpp = window.AppCore;
	    
        for( var i   = 0; i < elements.length; i++) {
            var e    = elements[i];
		
            trs.push($(e).data('i18n')); // ease translation process

            if ( $(e).html ) {
                $(e).html(cpp.jstr($(e).data('i18n')));
            } else if ( $(e).val ) {
                $(e).val(cpp.jstr($(e).data('i18n')));
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
        // end add-icons
    
        // ease translation process
        var utrs     = [];
        $.each(trs, function(i, el){
            if($.inArray(el, utrs) === -1) utrs.push(el);
        });
    
        var str      = '';
        for (var i   = 0; i < utrs.length; i++) {
            var tr   = utrs[i];
            str += 'tr("' + tr + '");\n';
        };
    
        //cpp.debug(str);
    }
};
