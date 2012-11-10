(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Totalizing %d questions': function(n) {
            return  'Totalizing ' + n + ' question' + (n==1?'':'s');
        },
        
        '__lang__': 'English'
    };
    
    AppI18N.registerTranslation('en', translation);
})();
