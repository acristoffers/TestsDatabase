var AppI18N = {
    availableLanguages: function() {
        var langs = {};
        for( key in AppI18N.languages) {
            var obj = AppI18N.languages[key];
            langs[obj['__lang__']] = key;
        }
        return langs;
    },
    
    currentLanguage: 'pt_BR',
    
    languages: {},
    
    registerTranslation: function(lang, obj) {
        AppI18N.languages[lang] = obj;
    },
    
    tr: function(text, numbers) {
        if( AppI18N.languages[AppI18N.currentLanguage][text] instanceof Function )
            return AppI18N.languages[AppI18N.currentLanguage][text](numbers)
        else if( AppI18N.languages[AppI18N.currentLanguage][text] )
            return AppI18N.languages[AppI18N.currentLanguage][text];
        else
            return text;
    },
    
    translate: function() {
        var elements = $('*').filter(function() {
            return $(this).data('i18n') != undefined; 
        });
	    
        for( var i   = 0; i < elements.length; i++) {
            var e    = elements[i];
            
            var text = $(e).data('i18n');
            var numbers = null;
            
            if ( $(e).data('i18n-numbers') != undefined) {
                numbers = $(e).data('i18n-numbers').toString().split(';');
            }
            
            if ( $(e).html ) {
                $(e).html(AppI18N.tr(text, numbers));
            } else if ( $(e).val ) {
                $(e).val(AppI18N.tr(text, numbers));
            }
        }
        
    	// begin add-icons
        var elements = $('*').filter(function() {
    		return $(this).data('icon') != undefined; 
    	});
    	for( var i = 0; i < elements.length; i++) {
            var e = elements[i];
        
            $(e).html( '<i class="icon-' + $(e).data('icon') + '"></i> ' + $(e).html());
        }
    }
};
