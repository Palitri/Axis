var AxStringComparison = Object.freeze(
{
    CaseSensitive : 0,
    CaseInsensitive : 1
});

function AxStrings()
{
    AxList.call(this);
}

AxStrings.prototype = Object.create(AxList.prototype);


AxStrings.prototype.Dispose = function()
{
};

/**
 * Splits a string by a given separator and fills the segments in
 * @param {AxString} str The original string which will be split
 * @param {AxString} splitter The splitter which acts as a delimiter between the segments of the original string
 * @param {Boolean} ignoreEmpty Determines whether to include any empty segments
 */
AxStrings.prototype.SplitString = function(str, splitter, ignoreEmpty)
{
    this.Clear();

    var start = 0;
    var end;
    do
    {
        end = str.IndexOf(splitter, start);
        if (end === -1)
            end = str.length;

        var length = end - start;
        if ((!ignoreEmpty) || (length > 0))
            this.Add(str.SubString(start, length));

        start = end + splitter.length;
    }
    while (start < str.length);
};

/**
 * Joins the strings into one single string
 * @param {!AxString} splitter A splitter to insert between the strings in the result. If omitted, a comma separator will be used
 * @return {AxString} A string with all the strings joined one after another, with an optional splitter between them
 */
AxStrings.prototype.ToString = function(splitter)
{
    if (AxUtils.IsUndefined(splitter))
        splitter = ',';

    var result = new AxString();
    for (var i = 0;;)
    {
        result.Insert(this.Get(i));

        if (++i === this.count)
            break;

        result.Insert(splitter);
    }

    return result;
};

/**
 * Gets the first occurence of a given string
 * @param {AxString} str The string to search for
 * @param {!AxStringComparison} comparisonMode Mode of comparison. If omitted, comparison will be case sensitive
 * @return {Integer} The index of the first occurence of the given string. If the string isn't found, the return value is -1
 */
AxStrings.prototype.IndexOf = function(str, comparisonMode)
{
    if (AxUtils.IsUndefined(comparisonMode))
        comparisonMode = AxStringComparison.CaseSensitive;
    
    if (comparisonMode === AxStringComparison.CaseSensitive)
    {
        for (var i = 0; i < this.count; i++)
            if (this.Get(i).Equals(str))
                return i;
    }
    else if (comparisonMode === AxStringComparison.CaseInsensitive)
    {
        var lower = str.ToLower();
        for (var i = 0; i < this.count; i++)
            if (this.Get(i).ToLower().Equals(lower))
                return i;
    }

    return -1;
};
