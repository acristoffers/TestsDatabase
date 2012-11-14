(function() {
    var translation = {
        formatNumber: function(number) {
            return number;
        },
        
        'Test %d': function(n) {
            return 'Test ' + n;
        },
        
        'There is not %d questions with difficulty between %d and %d on the selected categories': function(n) {
            return 'There is not ' + n[0] + (n[0]==1?' question':' questions') + ' with difficulty between ' + n[1] + ' and ' + n[2] + ' on the selected categories';
        },
        
        'Totalizing %d questions': function(n) {
            return  'Totalizing ' + n + ' question' + (n==1?'':'s');
        },
        
        'Version %d': function(n) {
            return 'Version ' + n;
        },
        
        '__lang__': 'English'
    };
    
    AppI18N.registerTranslation('en', translation);
})();
