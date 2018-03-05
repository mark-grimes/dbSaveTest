# sqlpp11 speed test

The compilation script is not great at the moment, I haven't put any effort into making it general.
[sqlpp](https://github.com/rbock/sqlpp11) is an SQL abstraction library that can connect to MySQL and SQLite
(amongst others), so I thought it give it a go since it's a lot more fully featured than my code in the customCpp
directory. It's aimed at cases for when the schema is known at compile time though, so it's not a great fit.

In my current test, it's about 6 times slower than customCpp but I'm still learning the library so I might
not be using it optimally. It also doesn't currently copy all tables since I have to code it statically
rather than have it loop over the input database tables dynamically at run time.

I have no intention to improve any of the code or scripts since it's a long way of the customCpp performance.

Note that the numbers below are on different hardware than for the times in the root README.

-------------------------------------------------------------------
| Method                     | Real       | User      | Sys       |
|----------------------------|------------|-----------|-----------|
| customCpp                  | 0m4.441s   | 0m2.493s  | 0m0.323s  |
| sqlpp11                    | 0m28.708s  | 0m24.231s | 0m3.449s  |
-------------------------------------------------------------------
