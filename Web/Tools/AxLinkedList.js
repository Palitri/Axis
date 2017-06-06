function AxLinkedListNode()
{
    this.previous = null;
    this.next = null;
    
    this.value = null;
};



function AxLinkedList()
{
    this.first = null;
    this.last = null;
    
    this.count = 0;
}

AxLinkedList.prototype.Dispose = function()
{
    this.Clear();
};

/**
 * Adds an element at the end of the list
 * @param {*} value Value of the element to add
 */
AxLinkedList.prototype.Add = function(value)
{
    this.InsertAfter(this.last, value);
};

/**
 * Returns whther the list is empty
 * @return {Boolean} True if the list is empty, i.e. has no elements
 */
AxLinkedList.prototype.IsEmpty = function()
{
    return this.last === null;
};

/**
 * Clears the list of all elements, leaving it empty
 */
AxLinkedList.prototype.Clear = function()
{
    while (this.last !== null)
        this.RemoveNode(this.last);
};

/**
 * Searches for the first occurrence of an element with the given value
 * @param {*} value The value of the element to search for
 * @return {AxLinkedListNode} The node containing the first element with the given value. If none such node is found, returns null
 */
AxLinkedList.prototype.FindFirst = function(value)
{
    var node = this.first;

    while (node !== null)
    {
        if (node.value === value)
            return node;

        node = node.next;
    }

    return null;
};

/**
 * Searches for the last occurrence of an element with the given value
 * @param {*} value The value of the element to search for
 * @return {AxLinkedListNode} The node containing the first element with the given value. If none such node is found, returns null
 */
AxLinkedList.prototype.FindLast = function(value)
{
    var node = this.last;

    while (node !== null)
    {
        if (node.value === value)
            return node;

        node = node.previous;
    }

    return null;
};

/**
 * Inserts an element just before a given node
 * @param {AxLinkedListNode} node The node, before which the new element is to be inserted
 * @param {*} value Value of the element to add
 */
AxLinkedList.prototype.InsertBefore = function(node, value)
{
    var listNode = node;

    var newNode = new AxLinkedListNode();
    newNode.value = value;

    newNode.next = listNode;
    if (listNode !== null)
    {
        newNode.previous = listNode.previous;
        if (listNode.previous !== null)
            listNode.previous.next = newNode;
        else
            this.first = newNode;
        listNode.previous = newNode;
    }
    else
    {
        newNode.previous = null;
        this.last = newNode;
        this.first = newNode;
    }

    this.count++;
};

/**
 * Inserts an element just after a given node
 * @param {AxLinkedListNode} node The node, after which the new element is to be inserted
 * @param {*} value Value of the element to add
 */
AxLinkedList.prototype.InsertAfter = function(node, value)
{
    var listNode = node;

    var newNode = new AxLinkedListNode();
    newNode.value = value;

    newNode.previous = listNode;
    if (listNode !== null)
    {
        newNode.next = listNode.next;
        if (listNode.next !== null)
            listNode.next.previous = newNode;
        else
            this.last = newNode;
        listNode.next = newNode;
    }
    else
    {
        newNode.next = null;
        this.last = newNode;
        this.first = newNode;
    }

    this.count++;
};

/**
 * Removes a node from the list
 * @param {AxLinkedListNode} node The node which is to be removed
 */
AxLinkedList.prototype.RemoveNode = function(node)
{
    var listNode = node;

    if (listNode.previous !== null)
        listNode.previous.next = listNode.next;
    else
        this.first = listNode.next;

    if (listNode.next !== null)
        listNode.next.previous = listNode.previous;
    else
        this.last = listNode.previous;

    listNode = null;

    this.count--;
};

/**
 * Gets the node which is located next to the given one
 * @param {AxLinkedListNode} node The node, for which to get the next one. If null, then the first node in the list is returned
 * @return {AxLinkedListNode} The node which is located next to the given one
 */
AxLinkedList.prototype.GetNodeNext = function(node)
{
    if (node !== null)
        return node.next;
    else
        return this.first;
};

/**
 * Gets the node which is located before the given one
 * @param {AxLinkedListNode} node The node, for which to get the previous one. If null, then the last node in the list is returned
 * @return {AxLinkedListNode} The node which is located before to the given one
 */
AxLinkedList.prototype.GetNodePrevious = function(node)
{
    if (node !== null)
        return node.previous;
    else
        return this.last;
};

/**
 * Gets the value of the given node
 * @param {AxLinkedListNode} node The node whose value to get
 * @return {*} The value of the given node
 */
AxLinkedList.prototype.GetNodeValue = function(node)
{
    return node.value;
};

/**
 * Sets the value for the given node
 * @param {AxLinkedListNode} node The node whose value to set
 * @param {*} value The value which is to be set for the node
 */
AxLinkedList.prototype.SetNodeValue = function(node, value)
{
    node.value = value;
};