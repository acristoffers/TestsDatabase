(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Totalizing %d questions': function(n) {
            return  'Totalizing ' + n + ' question' + (n==1?'':'s');
        },
        
        'There is not %d questions with difficulty between %d and %d on the selected categories': function(n) {
            return 'There is not ' + n[0] + ' questions with difficulty between ' + n[1] + ' and ' + n[2] + ' on the selected categories'
        },
        
        'Test %d': function(n) {
            return 'Test ' + n;
        },
        
        '__lang__': 'English'
    };
    
    AppI18N.registerTranslation('en', translation);
})();
