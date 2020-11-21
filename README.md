# Sad

Sad is a simple data file format, like Json, Toml ..., but more simpler.

Sad has three data types: **String**, **Array** and **Dictionary**, as **SAD**.

**String** is the primary data of Sad, **Array** is an orderd list and **Dictionary**
is a unorderd key-value list (or called dictionary).

A Sad *value* can be any one of these three things, array and dictionary can take
any sad values as members.

## Example

```sad
{
    string1 'string must be quoted'
    string2 "string must be quoted"
    array [
        'array members are in `[]`'
        [ 'members can be any type' ]
        {  }
        'even an empty dict'
    ]
    no-coma 'since Sad is such a simple format'
    no-semicolon 'that there is no need for comas or semicolons'
}
```
