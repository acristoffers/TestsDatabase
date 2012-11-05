var AppI18N = {
    availableLanguages: function() {
        var langs = {};
        for( key in AppI18N.languages) {
            var obj = AppI18N.languages[key];
            langs[obj['__lang__']] = key;
        }
        return langs;
    },
    
    currentLanguage: 'en',
    
    languages: {},
    
    registerTranslation: function(lang, obj) {
        AppI18N.languages[lang] = obj;
    },
    
    translate: function(text, numbers) {
        var tr;
        
        if( AppI18N.languages[AppI18N.currentLanguage][text] ){
            tr = AppI18N.languages[AppI18N.currentLanguage][text];
        } else {
            tr = text;
        }
        
        if ( numbers ) {
            if ( numbers instanceof Array ) {
                for (var i = numbers.length - 1; i >= 0; i--){
                    tr = tr.replace('%d', AppI18N.languages[AppI18N.currentLanguage].formatNumber(numbers[i]));
                }
            } else {
                tr = tr.replace('%d', AppI18N.languages[AppI18N.currentLanguage].formatNumber(numbers));
            }
        }
        return tr;
    }
};
