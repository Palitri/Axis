function AxLight()
{
    AxResource.call(this);

    this.typeId = AxLight.classId;
    this.resourceType = AxResourceType.Light;
       
    this.properties.Add(new AxProperty(new AxString('Color'), new AxVector4(1.0)));
    this.properties.Add(new AxProperty(new AxString('Intensity'), 1000.0));
}

AxLight.prototype.Dispose = function()
{
};

AxLight.prototype = Object.create(AxResource.prototype);


AxLight.classId = (AxResourceType.Light << 16) | 0;

AxLight.propertyIndex_Color     = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxLight.propertyIndex_Intensity = AxResource.propertyIndex_ChildPropertiesIndex + 1;
