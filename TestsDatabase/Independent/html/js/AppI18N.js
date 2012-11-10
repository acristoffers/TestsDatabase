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
        if( AppI18N.languages[AppI18N.currentLanguage][text] instanceof Function )
            return AppI18N.languages[AppI18N.currentLanguage][text](numbers)
        else if( AppI18N.languages[AppI18N.currentLanguage][text] )
            return AppI18N.languages[AppI18N.currentLanguage][text];
        else
            return text;
    }
};
