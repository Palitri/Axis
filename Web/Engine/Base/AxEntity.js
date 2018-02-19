/**
 * This is a virtual prototype meant to be inherited by overrideing its methods
 * Creates a new entity object
 * Entities are fundamenal serializable objects, building blocks of the axis scenery
 * @constructor
 */
function AxEntity()
{
    // typeId can be assigned to a static member of a AxClassId class, which in its constructor can assign a unique AxClassId.id. This way we could have, for example AxMesh.typeId = AxMesh::classId.id. However it would be a bit obscure, because even if this will ensure the uniqueness of the ids, it wont ensure that the same class will always have the same id assigned, which is vital for serialization.
    this.typeId = 0;
}

/**
 * Virtual funciton
 * Serializes the entity into the stream
 * @param {AxStream} destination Stream in which to write the serialized data of the entity
 */
AxEntity.prototype.Serialize = function(destination) { };

/**
 * Virtual funciton
 * Deserializes the entity, reading its data from a source stream
 * @param {AxStream} source Stream from which to read the serialized data of the entity
 * @return {Boolean} True if the deserialization was successfull
 */
AxEntity.prototype.Deserialize = function(source) { };
        