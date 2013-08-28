@[exports set :if ^(){
    [bind :result]
    [bind :args arguments]
    [bind :continue true] [loop ^[and continue [> @[args length] 0]] ^{
        [[? [> @[args length] 1] ^{
              [bind :condition @[args el 0]]
              [bind :body @[args el 1]]
              [[? [condition] ^{
                    [set :result [body]]
                    [set :continue false]
                  } ^{
                    [set :args @[args slice 2]]
              }]]
            } ^{
              [set :result [@[args el 0]]]
              [set :continue false]
        }]]
    }]
    result
}]

@[exports set :branch ^(:on){
    [bind :result]
    [bind :conditions arguments]
    [bind :continue true] [bind :i 1] [loop ^[and continue [< i @[conditions length]]] ^{
        [bind :case @[conditions el i]]
        [if ^@[case is? List] ^{
            [if ^[= on @[case el 0]] ^{
                [set :result [@[case el 1]]]
                [set :continue false]
            }]
          } ^{
            [set :result [case]]
            [set :continue false]
        }]
        [set :i [+ i 1]]
    }]
    result
}]
