/**
 * Creates a new list of items
 * @param {!*} value 
 * @param {!Integer} numValues
 * @constructor
 */
function AxList(value, numValues)
{
    this.count = 0;
    this.capacity = 0;
    this.values = null;
    
    if (AxUtils.IsInteger(value))
    {
        this.SetCapacity(value);
        return;
    }
    else if ((typeof value === 'AxList') || Array.isArray(value))
    {
        this.Copy(value, numValues);
    }
}

AxList.ListIndex_NotFound = -1;

/**
 * Copies a given number of items from anoher list
 * @param {AxList|Array} values The AxList or Array to copy values from
 * @param {Integer} numValues The number of values to copy
 */
AxList.prototype.Copy = function(values, numValues)
{
    this.Copy_2(0, values, 0, numValues);
};

/**
 * Copies a given number of items from anoher list
 * @param {Integer} offset The offset index at which to start copying the values into the list
 * @param {AxList|Array} values The AxList or Array to copy values from
 * @param {Integer} valuesOffset The index offset at which to start copying from the source values
 * @param {Integer} numValues The number of values to copy
 */
AxList.prototype.Copy_2 = function(offset, values, valuesOffset, numValues)
{
    if (typeof values === 'AxList')
        values = values.values;
    
    if (AxUtils.IsUndefinedOrNull(numValues))
        numValues = values.length;

    this.SetSize(numValues);

    for (var i = 0; i < numValues; i++)
        this.values[offset + i] = values[valuesOffset + i];
};

/**
 * Gets the value at the given index
 * @param {Integer} index The index at which to get a value
 * @return {*} The value at the given index
 */
AxList.prototype.Get = function(index) 
{ 
    return this.values[index]; 
};

/**
 * Sets the value at the given index
 * @param {Integer} index The index at which to set a value
 * @param {*} value The value to set at the given index in the list
 */
AxList.prototype.Set = function(index, value) 
{ 
    this.values[index] = value;
};

/**
 * Returns the number of items in the list
 * @return {Integer} The number of items in the list
 */
AxList.prototype.Size = function()
{
    return this.count;
};

/**
 * Sets the lists' capacity to hold the given number of items without need to allocate new memory
 * The contents of the list are kept
 * @param {Integer} newCapacity The new capacity to set for the list
 */
AxList.prototype.SetCapacity = function(newCapacity)
{
    var newValues = newCapacity > 0 ? new Array(newCapacity) : null;

    var count = this.count < newCapacity ? this.count : newCapacity;
    for (var i = 0; i < count; i++)
        newValues[i] = this.values[i];

    this.values = newValues;
    this.capacity = newCapacity;
};

/**
 * If necessary, sets such a capacity of the list, so that it will be able to hold at least the given number of items without need to allocate new memory
 * @param {Integer} capacity The capacity to set for the list
 */
AxList.prototype.EnsureCapacity = function(capacity)
{
    var newCapacity = this.capacity > 0 ? this.capacity : 1;
    while (capacity > newCapacity)
    {
        newCapacity *= 2;
    }

    if (newCapacity > this.capacity)
    {
        this.SetCapacity(newCapacity);
    }	
};

/**
 * Sets the size of the list to the number of items
 * @param {Integer} numElements The size in number of items to set
 */
AxList.prototype.SetSize = function(numElements)
{
    this.EnsureCapacity(numElements);
    this.count = numElements;
};

/**
 * Clears the list. This method only sets the items counter to 0, without actually erasing data. Further insertion operations will put new items in place of the old ones.
 */
AxList.prototype.Clear = function()
{
    this.count = 0;
};

/**
 * Adds an item with the given value to the end of the list
 * @param {*} value The value to add to the list
 * @return {*} The value which was added
 */
AxList.prototype.Add = function(value)
{
    this.EnsureCapacity(this.count + 1);

    this.values[this.count] = value;

    return this.values[this.count++];
};

/**
 * Inserts an item with the given value at the specified index of the list
 * @param {Integer} index Index at which to insert the new item
 * @param {*} value The value to insert into the list
 * @return {*} The value which was inserted
 */
AxList.prototype.Insert = function(index, value)
{
    this.EnsureCapacity(this.count + 1);

    for (var i = this.count; i > index; i--)
        this.values[i] = this.values[i - 1];

    this.values[index] = value;

    this.count++;

    return this.values[index];
};

/**
 * Inserts a number of items form another list at the specified index
 * @param {Integer} index Index at which to insert the new items
 * @param {AxList|Array} values values to insert
 */
AxList.prototype.InsertRange = function(index, values)
{
    if (typeof values === 'AxList')
        values = values.values;
    
    var shift = values.length;
    
    this.EnsureCapacity(this.count + shift);

    for (var i = this.count; i > index; i--)
        this.values[i] = this.values[i - shift];

    for (var i = 0; i < shift; i++)
        this.values[index + i] = values[i];

    this.count += shift;
};


/**
 * Moves items from their original position and inserts them on a new position, shifting the items between
 * @param {Integer} originalIndex Original index at which the items to be moved begin
 * @param {Integer} newIndex New index at which the items are to be moved
 * @param {Integer} count Number of items to move
 */
AxList.prototype.Move = function(originalIndex, newIndex, count)
{
    if ((count <= 0) || (originalIndex === newIndex))
        return;
    
    temp = new Array[count];
    for (var i = 0; i < count; i++)
        temp[i] = this.values[originalIndex + i];

    if (originalIndex < newIndex)
    {
        for (var i = originalIndex; i < newIndex; i++)
            this.values[i] = this.values[i + count];
    }
    else
    {
        for (var i = originalIndex - 1; i >= newIndex; i--)
            this.values[i + count] = this.values[i];
    }
    
    for (var i = 0; i < count; i++)
        this.values[newIndex + i] = temp[i];
};

/**
 * Removes the first encountered item of the given value. Returns true if an item was removed or false otherwise
 * @param {*} value The value to be removed at first occuring item of the list
 * @return {Boolean} True if an item with the given value was removed from the array
*/
AxList.prototype.Remove = function(value)
{
    var index = this.IndexOf(value);
    if (index !== AxList.ListIndex_NotFound)
    {
        this.RemoveAt(index);
        return true;
    }

    return false;
};

/**
 * Removes all items of the given value. Returns the number of items removed
 * @param {*} value The value to be removed from the items of the list
 * @return {Integer} The number of items removed
 */
AxList.prototype.RemoveAll = function(value)
{
    var numRemoved = 0;
    var index = this.IndexOf(value);
    while (index !== AxList.ListIndex_NotFound)
    {
        this.RemoveAt(index);
        numRemoved++;

        index = this.IndexOf(value);
    }

    return numRemoved;
};

/**
 * Removes the item at the given index
 * @param {Integer} index The index at which to remove an item
 */
AxList.prototype.RemoveAt = function(index)
{
    for (var i = index; i < this.count -1; i++)
            this.values[i] = this.values[i + 1];
    this.count--;
};

/**
 * Removes a number of consequent items from the list, starting from the given index
 * @param {Integer} index The index at which to remove the items
 * @param {Integer} count The number of items to remove
 */
AxList.prototype.RemoveRange = function(index, count)
{
    for (var i = index; i < this.count - count; i++)
        this.values[i] = this.values[i + count];
    this.count -= count;
};

/**
 * Returns the index of the first encountered item with the given value. If such item is not present, the return value is AxList.ListIndex_NotFound
 * @param {*} value The value of the item to search for
 * @return {Integer} The index of the item where the given value was first encountered. If no such item was found, return value is AxList.ListIndex_NotFound
 */
AxList.prototype.IndexOf = function(value)
{
    for (var i = 0; i < this.count; i++)
        if (this.values[i] === value)
            return i;

    return AxList.ListIndex_NotFound;
};

/**
 * Returns the index of the last encountered item with the given value. If such item is not present, the return value is AxList.ListIndex_NotFound
 * @param {*} value The value of the item to search for
 * @return {Integer} The index of the item where the given value was last encountered. If no such item was found, return value is AxList.ListIndex_NotFound
 */
AxList.prototype.LastIndexOf = function(value)
{
    for (var i = this.count - 1; i >= 0; i--)
        if (this.values[i] === value)
            return i;

    return AxList.ListIndex_NotFound;
};


/**
 * Returns the the number of occurences of the given value in the list
 * @param {*} value The value of the item to count
 * @return {Integer} The number of occurences of the value in the list
 */
AxList.prototype.Count = function(value)
{
    var result = 0;
    for (var i = 0; i < this.count; i++)
        if (this.values[i] === value)
            result++;

    return result;
};

/**
 * Returns whether the list contains at least one item of the given value
 * @param {*} value The value of the item to look for
 * @return {Boolean} True if there is at least one item if the given value in the list
 */
AxList.prototype.Contains = function(value)
{
    for (var i = 0; i < this.count; i++)
        if (this.values[i] === value)
            return true;

    return false;
};