function AxDictionaryEntry()
{
    this.key = null;
    this.value = null;
}


function AxDictionary()
{
    AxLinkedList.call(this);
}

AxDictionary.prototype = Object.create(AxLinkedList.prototype);


AxDictionary.prototype.Dispose = function()
{
};

/**
 * For internal use
 * Searches for a node with the given key. If no such node is available, returns a the node with the next bigger key value
 * @param {*} key The key, whose node to search for
 * @param {AxReferenceParameter} node A AxDictionaryEntry reference, which outputs the node with the given key or if the key does not exist, the node with the next biggest key
 * @return {AxDictionaryEntry} The entry in the dictionary with the given key, or if such does not exist, th entry with the next biggest key
 */
AxDictionary.prototype.FindEntryNode = function(key, node)
{
    for (node.value = this.GetNodeNext(null); node.value !== null; node.value = this.GetNodeNext(node.value))
    {
        var entry = this.GetNodeValue(node.value);
        if (entry.key === key)
            return true;

        if (entry.key > key)
            return false;
    }

    return false;
};


/**
 * Adds a value with the given key. If the key already exists, the method returns false, true otherwise
 * @param {*} key A key for the entry to add. If the key is alredy present, the dictionary remains unaltered.
 * @param {*} value A value for the entry to add
 * @return {Boolean} True if the key and value were successfully added. False otherwise, for example if the key already exists in the dictionary
 */
AxDictionary.prototype.Add = function(key, value)
{
        var nextBiggest = new AxReferenceParameter(null);
        if (this.FindEntryNode(key, nextBiggest))
            return false;

        nextBiggest = nextBiggest.value;
        
        var entry = new AxDictionaryEntry();
        entry.key = key;
        entry.value = value;
        if (nextBiggest !== null)
            this.InsertBefore(nextBiggest, entry);
        else
            this.InsertAfter(this.last, entry);

        return true;
};

/**
 * Sets a value with the give key. If the key already exists, it updates its value with the provided one and returns false. Otherwise it adds the new key with the value and returns true
 * @param {*} key The key of the entry to set. If the key is not present, a new entry is created. If the key is alredy present, the value for the entry gets updated with the given one
 * @param {*} value A value for the entry
 * @return {Boolean} True if a an entry with the given key did not exist and a new entry was created. False if only the value of an existing entry was updated.
 */
AxDictionary.prototype.Set = function(key, value)
{
    var node = new AxReferenceParameter(null);
    if (this.FindEntryNode(key, node))
    {
        node = node.value;

        var entry = new AxDictionaryEntry();
        entry.key = key;
        entry.value = value;

        this.SetNodeValue(node, entry);
        return false;
    }

    node = node.value;

    var entry = new AxDictionaryEntry();
    entry.key = key;
    entry.value = value;
    if (node !== null)
        this.InsertBefore(node, entry);
    else
        this.InsertAfter(this.last, entry);

    return true;
};

/**
 * Removes the given key with its value. Returns true if the given key was existing and was removed, false if the key didn't exist
 * @param {*} key The key, whose entry to remove
 * @return {Boolean} True if an entry with the given key existed and was removed.
 */
AxDictionary.prototype.Remove = function(key)
{
    var entry = new AxReferenceParameter(null);
    if (!this.FindEntryNode(key, entry))
        return false;

    entry = entry.value;    
    this.RemoveNode(entry);

    return true;
};

/**
 * Gets the value for the given key
 * @param {*} value The key whose respective value to get
 * @param {AxReferenceParameter} key A reference, which outputs the value of the entry with the given key.
 * @return {Boolean} True if an entry with the given key exists and the value was successfully acquired
 */
AxDictionary.prototype.GetValue = function(key, value)
{
    var entry = new AxReferenceParameter(null);
    if (!this.FindEntryNode(key, entry))
        return false;

    entry = entry.value;    
    value.value = this.GetNodeValue(entry).value;

    return true;
};

/**
 * Checks whether an entry with a given key exists in the dictionary
 * @param {*} key The key to check for
 * @return {Boolean} True if an entry with the given key exists
 */
AxDictionary.prototype.KeyExists = function(key)
{
    var entry = new AxReferenceParameter(null);
    return this.FindEntryNode(key, entry);
};

/**
 * Checkes whether an entry with a given value exists in the dictionary
 * @param {*} value The value to check for
 * @return {Boolean} True if an entry with the given value exists
 */
AxDictionary.prototype.ValueExists = function(value)
{
    for (var entry = this.GetNodeNext(null); entry !== null; entry = this.GetNodeNext(entry))
    {
        if (this.GetNodeValue(entry).value === value)
            return true;
    }

    return false;
};