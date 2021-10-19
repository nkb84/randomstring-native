# randomstring-native

Simple Module for generating Random Strings by native.

Normally, generate random string is easy, but it's not good performance if need to generate a huge random string at once

## Getting Started
Install the module with: `npm install randomstring-native`

## Documentation

### options

#### options.length

number - the length of your resulting string (DEFAULT: 10)

#### options.count

number - number string to be generated (DEFAULT: 1)

#### options.charset

string - character set (DEFAULT: '0123456789abcdefghijklmnopqrstuvwxyz')

#### options.method

string - random string generation method. It can be 'mt19937', 'crypto' or 'random' (DEFAULT: 'crypto')

#### options.excludes

array - the string generated will not appear in this list

## Examples

```javascript
const generator = require('randomstring-native');
// Generate 1000000 random strings
let uids = generator.generate({
        'charset': '0123456789abcdefghijklmnopqrstuvxyz',
        'length': 10,
        'count': 1000000,
        'method': 'random',
        'excludes': ['003fy23h5m', '003g18hjv7']
    })
```

## License
Copyright (c) 2021 nkb84
Licensed under the MIT license.