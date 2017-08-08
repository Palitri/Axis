function AxStack(capacity)
{
    if (AxUtils.IsUndefinedOrNull(capacity))
        capacity = 8;

    AxList.call(this, capacity);
}

AxStack.prototype = Object.create(AxList.prototype);



/**
 * Pushes a new iteam to the top of the stack
 * @param {*} value Value of the item to be pushed
 * @returns {*} The pushed value
 */
AxStack.prototype.Push = function(value)
{
    return this.Add(value);
};

/**
 * Pops an item from the top of the stack and returns its value
 * @returns {*} The values which was popped from the top of the stack
 */
AxStack.prototype.Pop = function()
{
    return this.values[--this.count];
};

/**
 * Peeks the item at the top of the stack and returns its value without popping it out
 * @returns {AxStack.values} The value of the item at the top of the stack
 */
AxStack.prototype.Peek = function()
{
    return this.values[this.count - 1];
};
        