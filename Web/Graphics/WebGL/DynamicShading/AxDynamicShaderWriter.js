/**
 * Creates a new dynamic shader writer object
 * The dynamic shader writer is used to generate the graphics device's shading program code in compliance with Axis' shading layers architecture
 * @param {Boolean} animated Denotes whether the shader should provide skeletal animation (skinning)
 * @param {Integer} maxBones The maximum number of bones in case the shader is animated
 * @constructor
 */
function AxDynamicShaderWriter(animated, maxBones)
{
    this.vsInputType = new AxDynamicShaderBlockDefinitions();
    this.vsParams = new AxDynamicShaderBlockDefinitions();
    this.vsCode = new AxDynamicShaderBlockDefinitions();
    this.sIOType = new AxDynamicShaderBlockDefinitions();
    this.psParams = new AxDynamicShaderBlockDefinitions();
    this.psCode = new AxDynamicShaderBlockDefinitions();
    this.psOutputType = new AxDynamicShaderBlockDefinitions();

    this.animated = animated;
    this.maxBones = maxBones;

    this.numLights = 0;
    this.textureSlots = 0;
    this.texCoordSlots = 0;
    
    this.shadingOps = new AxList();
    

    if (this.animated)
    {
        this.vsParams.ProvideDefinition('bones', 'uniform mat4 worldMatrix[' + this.maxBones + '];\r\n');
    }
    else
    {
        this.vsParams.ProvideDefinition('worldMatrix', 'uniform mat4 worldMatrix;\r\n');
    }

    this.vsParams.ProvideDefinition('', 'precision mediump float;');
    this.vsParams.ProvideDefinition('viewProjectionMatrix', 'uniform mat4 viewProjectionMatrix;');//
    this.vsParams.ProvideDefinition('ambientLight', 'uniform vec4 ambientLight;');
    this.vsParams.ProvideDefinition('viewPosition', 'uniform vec3 viewPosition;');
    
    this.psParams.ProvideDefinition('', 'precision mediump float;');
    this.psParams.ProvideDefinition('viewPosition', 'uniform vec3 viewPosition;');

    this.vsInputType.ProvideDefinition('input_pos', 'attribute vec3 input_pos;');
    if (this.animated)
    {
        this.vsInputType.ProvideDefinition('input_boneIndices', 'attribute vec4 input_boneIndices;');
        this.vsInputType.ProvideDefinition('input_boneWeights', 'attribute vec4 input_boneWeights;');
    }

    this.sIOType.ProvideDefinition('pos', 'varying vec4 sio_pos;');

    this.psParams.ProvideDefinition('', 'precision mediump float;');
    this.psParams.ProvideDefinition('ambientLight', 'uniform vec4 ambientLight;');
    
    this.psOutputType.ProvideDefinition('PSOutput', 'struct PSOutput\r\n{');
    this.psOutputType.ProvideDefinition('color', '\tvec4 color;');

    this.vsCode.ProvideDefinition('', 'void main()\r\n{');
    if (this.animated)
    {
        this.vsCode.ProvideDefinition('bonesMatrix', '\tmat4 bonesMatrix;');
        this.vsCode.ProvideDefinition('', '\tbonesMatrix = \r\n' +
            '\t\tworldMatrix[int(input_boneIndices.x)] * input_boneWeights.x +\r\n' +
            '\t\tworldMatrix[int(input_boneIndices.y)] * input_boneWeights.y +\r\n' +
            '\t\tworldMatrix[int(input_boneIndices.z)] * input_boneWeights.z +\r\n' +
            '\t\tworldMatrix[int(input_boneIndices.w)] * input_boneWeights.w;');

        this.vsCode.ProvideDefinition('worldPos', '\tvec4 worldPos = bonesMatrix * vec4(input_pos, 1.0);\r\n');
    }
    else
    {
        this.vsCode.ProvideDefinition('worldPos', '\tvec4 worldPos = worldMatrix * vec4(input_pos, 1.0);\r\n');
    }
    this.vsCode.ProvideDefinition('.pos', '\tsio_pos = viewProjectionMatrix * worldPos;\r\n');

    this.psCode.ProvideDefinition('', 'void main()\r\n{');
    this.psCode.ProvideDefinition('result', '\tPSOutput result;\r\n');
}


AxDynamicShaderWriter.prototype.Dispose = function()
{
};

/**
 * Adds a shading layer to the shader writer
 * @param {AxShadingOperation} shadingOp Shading operation for the layer to add
 * @param {AxColorOperation} colorOp Color operation for the layer
 */
AxDynamicShaderWriter.prototype.AddShadingLayer = function(shadingOp, colorOp)
{
    
    switch (shadingOp)
    {
    //*
        case AxShadingOperation.BasicShading:
        {
            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition('color', 'varying vec4 sio_color;');

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');

            this.vsCode.ProvideDefinition('', '\tfloat intensity = clamp(-normal.z, 0.0, 1.0);\r\n');
            this.vsCode.ProvideDefinition('', '\tsio_color = vec4(intensity, intensity, intensity, 1.0);\r\n');

            this.psCode.ProvideDefinition('', '\tresult.color = sio_color;\r\n');
            //this.ProvideColorOperationCode(shadingOp, colorOp, '', this.psCode, '\t', 'result.color', 'sio_color', colorOp);
            break;
        }

        case AxShadingOperation.FlatColor:
        {
            var pFlatColor = this.ProvideParameterDefinition(this.psParams, shadingOp, "flatColor", AxParameterType.Color);
            
            this.ProvideColorOperationCode(shadingOp, colorOp, "flatColorAmount", this.psCode, "\t", "result.color", pFlatColor);
            
            break;
        }

        case AxShadingOperation.Tint:
        {
            var pTintColor = this.ProvideParameterDefinition(this.psParams, shadingOp, "tintColor", AxParameterType.Color);

            this.psCode.ProvideDefinition("tintFactor", "float tintFactor;");
            this.psCode.ProvideDefinition("", "\ttintFactor = (result.color.x + result.color.y + result.color.z) / 3.0;");

            this.ProvideColorOperationCode(shadingOp, colorOp, "tintAmount", this.psCode, "\t", "result.color", pTintColor + " * vec4(tintFactor, tintFactor, tintFactor, 1.0)");

            break;
        }

        case AxShadingOperation.AmbientLighting:
        {
            this.ProvideColorOperationCode(shadingOp, colorOp, "ambientAmount", this.psCode, "\t", "result.color", "ambient * ambientLight");
            
            break;
        }

        case AxShadingOperation.LightSource:
        {
            this.numLights++;
            break;
        }

        case AxShadingOperation.VertexLighting:
        {
            if (this.numLights === 0)
                break;

            if (this.shadingOps.Count(AxShadingOperation.VertexLighting) > 0)
                break;

            this.ProvideParameterDefinition(this.vsParams, shadingOp, "activeLightsCount", AxParameterType.Float);

            
            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition('color', 'varying vec4 sio_color;');

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
            
            this.vsCode.ProvideDefinition('viewVector', '\tvec3 viewVector = normalize(viewPosition - worldPos.xyz);');
            this.vsCode.ProvideDefinition('lightVector', '\tvec3 lightVector;');
            this.vsCode.ProvideDefinition('lightReflectionVector', '\tvec3 lightReflectionVector;');
            this.vsCode.ProvideDefinition('lightAttenuation', '\tfloat lightAttenuation;');
            this.vsCode.ProvideDefinition('lightDiffuse', '\tfloat lightDiffuse;');
            this.vsCode.ProvideDefinition('lightSpecular', '\tfloat lightSpecular;');
            this.vsCode.ProvideDefinition('color', '\tvec4 color;');
            this.vsCode.ProvideDefinition('', 
                '\tcolor = vec4(0.0, 0.0, 0.0, 0.0);\r\n' +
                '\tint iActiveLightsCount = int(activeLightsCount);\r\n' +
                '\tfor (int lightIndex = 0; lightIndex < maxLights; lightIndex++)\r\n' +
                '\t{\r\n' +
                '\t\tlightVector = lightsPosIntensity[lightIndex].xyz - worldPos.xyz;\r\n' +
                '\t\tlightAttenuation = 1.0 / length(lightVector);\r\n' +
                '\t\tlightVector *= lightAttenuation;\r\n' +
                '\t\tlightAttenuation *= lightAttenuation;\r\n' +
                '\t\tlightDiffuse = clamp(dot(normal, lightVector), 0.0, 1.0) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n' +
                '\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n' +
                '\t\tlightSpecular = pow(clamp(dot(normal, lightReflectionVector), 0.0, 1.0), gloss);\r\n' +
                '\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n' +
                '\t\tiActiveLightsCount--;\r\n' +
                '\t\tif (iActiveLightsCount == 0)\r\n' +
                '\t\t\t break;\r\n' +
                '\t}\r\n'
                );
            this.vsCode.ProvideDefinition('', '\tsio_color = color;');

            this.ProvideColorOperationCode(shadingOp, colorOp, 'vertexLightingAmount', this.psCode, '\t', 'result.color', 'sio_color', colorOp);
            break;
        }


        case AxShadingOperation.PixelLighting:
        {
            if (this.numLights === 0)
                break;

            if (this.shadingOps.Count(AxShadingOperation.PixelLighting) > 0)
                break;

            this.ProvideParameterDefinition(this.psParams, shadingOp, "activeLightsCount", AxParameterType.Int);

            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition("normal", "varying vec3 sio_normal;");
            this.sIOType.ProvideDefinition("worldPos", "varying vec4 sio_worldPos;");

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
            this.vsCode.ProvideDefinition(".normal", "\tsio_normal = normal;");
            this.vsCode.ProvideDefinition(".worldPos", "\tsio_worldPos = worldPos;");

            this.psCode.ProvideDefinition('sio2_normal', '\tvec3 sio2_normal = sio_normal;');

            this.psCode.ProvideDefinition('viewVector', '\tvec3 viewVector = normalize(viewPosition - sio_worldPos.xyz);');
            this.psCode.ProvideDefinition('lightVector', '\tvec3 lightVector;');
            this.psCode.ProvideDefinition('lightReflectionVector', '\tvec3 lightReflectionVector;');
            this.psCode.ProvideDefinition('lightAttenuation', '\tfloat lightAttenuation;');
            this.psCode.ProvideDefinition('lightDiffuse', '\tfloat lightDiffuse;');
            this.psCode.ProvideDefinition('lightSpecular', '\tfloat lightSpecular;');
            this.psCode.ProvideDefinition('color', '\tvec4 color;');
            this.psCode.ProvideDefinition('', 
                '\tcolor = vec4(0.0, 0.0, 0.0, 0.0);\r\n' +
                '\tint iActiveLightsCount = int(activeLightsCount);\r\n' +
                '\tfor (int lightIndex = 0; lightIndex < maxLights; lightIndex++)\r\n' +
                '\t{\r\n' +
                '\t\tlightVector = lightsPosIntensity[lightIndex].xyz - sio_worldPos.xyz;\r\n' +
                '\t\tlightAttenuation = 1.0 / length(lightVector);\r\n' +
                '\t\tlightVector *= lightAttenuation;\r\n' +
                '\t\tlightAttenuation *= lightAttenuation;\r\n' +
                '\t\tlightDiffuse = clamp(dot(sio2_normal, lightVector), 0.0, 1.0) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n' +
                '\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n' +
                '\t\tlightSpecular = pow(clamp(dot(sio2_normal, lightReflectionVector), 0.0, 1.0), gloss);\r\n' +
                '\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n' +
                '\t\tiActiveLightsCount--;\r\n' +
                '\t\tif (iActiveLightsCount == 0)\r\n' +
                '\t\t\t break;\r\n' +
                '\t}\r\n'
                );

            this.ProvideColorOperationCode(shadingOp, colorOp, "pixelLightingAmount", this.psCode, "\t", "result.color", "color");
            break;
        }
                
        case AxShadingOperation.ColorMap:
        {
            var pColorMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "colorMap", AxParameterType.ReferenceTexture);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');

            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');

            this.psCode.ProvideDefinition('sio2_texCoords', 'vec2 sio2_texCoords = sio_texCoords;');

            this.ProvideColorOperationCode(shadingOp, colorOp, 'colorMapAmount', this.psCode, '\t', 'result.color', AxDynamicShaderWriter.TextureSampling(pColorMap, 'sio2_texCoords'), colorOp);

            break;
        }

        case AxShadingOperation.ReflectionMap:
        {
            var pReflectionMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "reflectionMap", AxParameterType.ReferenceTexture);
            var pReflectionFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "reflectionMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition('normal', 'varying vec3 sio_normal;');
            this.sIOType.ProvideDefinition('screenSpaceNormal', 'varying vec3 sio_screenSpaceNormal;');
            this.sIOType.ProvideDefinition('screenSpacePos', 'varying vec3 sio_screenSpacePos;');

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
            this.vsCode.ProvideDefinition("wvpNormal", "\tvec3 wvpNormal = (viewProjectionMatrix * vec4(normal, 0.0)).xyz;");
            this.vsCode.ProvideDefinition(".normal", "\tsio_normal = normal;");
            this.vsCode.ProvideDefinition(".screenSpaceNormal", "\tsio_screenSpaceNormal = wvpNormal;");
            this.vsCode.ProvideDefinition(".screenSpacePos", 
                "\tsio_screenSpacePos = ((sio_pos.xyz / sio_pos.w) - 1.0) / 2.0;\r\n");

            this.psCode.ProvideDefinition('sio2_normal', '\tvec3 sio2_normal = sio_normal;');
            this.psCode.ProvideDefinition('sampleTexCoords', '\tvec2 sampleTexCoords;');
            this.psCode.ProvideDefinition('', '\tsampleTexCoords = sio_screenSpacePos.xy - sio2_normal.xy * ' + pReflectionFactor + ';');
            
            this.ProvideColorOperationCode(shadingOp, colorOp, 'reflectionMapAmount', this.psCode, '\t', 'result.color', AxDynamicShaderWriter.TextureSampling(pReflectionMap, 'sampleTexCoords'));
            break;
        }

        case AxShadingOperation.RefractionMap:
        {
            var pRefractionMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "refractionMap", AxParameterType.ReferenceTexture);
            var pRefractionMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "refractionMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition('normal', 'varying vec3 sio_normal;');
            this.sIOType.ProvideDefinition('screenSpaceNormal', 'varying vec3 sio_screenSpaceNormal;');
            this.sIOType.ProvideDefinition('screenSpacePos', 'varying vec3 sio_screenSpacePos;');

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
            this.vsCode.ProvideDefinition("wvpNormal", "\tvec3 wvpNormal = (viewProjectionMatrix * vec4(normal, 0.0)).xyz;");
            this.vsCode.ProvideDefinition(".normal", "\tsio_normal = normal;");
            this.vsCode.ProvideDefinition(".screenSpaceNormal", "\tsio_screenSpaceNormal = wvpNormal;");
            this.vsCode.ProvideDefinition(".screenSpacePos", 
                "\tsio_screenSpacePos = ((sio_pos.xyz / sio_pos.w) - 1.0) / 2.0;\r\n");

            this.psCode.ProvideDefinition('sio2_normal', '\tvec3 sio2_normal = sio_normal;');
            this.psCode.ProvideDefinition('sampleTexCoords', '\tvec2 sampleTexCoords;');
            this.psCode.ProvideDefinition('', '\tsampleTexCoords = sio_screenSpacePos.xy - sio2_normal.xy * ' + pRefractionMapFactor + ';');

            this.ProvideColorOperationCode(shadingOp, colorOp, 'refractionMapAmount', this.psCode, '\t', 'result.color', AxDynamicShaderWriter.TextureSampling(pRefractionMap, 'sampleTexCoords'));

            break;
        }
        
        case AxShadingOperation.NormalMap:
        {
            var pNormalMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "normalMap", AxParameterType.ReferenceTexture);
            var pNormalMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "normalMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');
            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');
            this.vsInputType.ProvideDefinition('tangent', 'attribute vec3 input_tangent;');
            this.vsInputType.ProvideDefinition('biTangent', 'attribute vec3 input_biTangent;');

            this.sIOType.ProvideDefinition('normal', 'varying vec3 sio_normal;');
            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');
            this.sIOType.ProvideDefinition('tangent', 'varying vec3 sio_tangent;');
            this.sIOType.ProvideDefinition('biTangent', 'varying vec3 sio_biTangent;');

            if (this.animated)
            {
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
                this.vsCode.ProvideDefinition('tangent', '\tvec3 tangent = normalize(bonesMatrix * vec4(input_tangent, 0.0)).xyz;');
                this.vsCode.ProvideDefinition('biTangent', '\tvec3 biTangent = normalize(bonesMatrix * vec4(input_biTangent, 0.0)).xyz;');
            }
            else
            {
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
                this.vsCode.ProvideDefinition('tangent', '\tvec3 tangent = normalize(worldMatrix * vec4(input_tangent, 0.0)).xyz;');
                this.vsCode.ProvideDefinition('biTangent', '\tvec3 biTangent = normalize(worldMatrix * vec4(input_biTangent, 0.0)).xyz;');
            }

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');
            this.vsCode.ProvideDefinition('.normal', '\tsio_normal = normal;');
            this.vsCode.ProvideDefinition('.tangent', '\tsio_tangent = tangent;');
            this.vsCode.ProvideDefinition('.biTangent', '\tsio_biTangent = biTangent;');

            this.psCode.ProvideDefinition('sio2_texCoords', '\tvec2 sio2_texCoords = sio_texCoords;');
            this.psCode.ProvideDefinition('sio2_normal', '\tvec3 sio2_normal;');
            this.psCode.ProvideDefinition("",  
                '\tvec3 mapNormal = (' + AxDynamicShaderWriter.TextureSampling(pNormalMap, 'sio2_texCoords') + '.xyz * 2.0 - 1.0) * ' + pNormalMapFactor + ';\r\n' +
                "\tsio2_normal = mapNormal.x * sio_tangent + mapNormal.y * sio_biTangent + mapNormal.z * sio_normal;\r\n");

            break;
        }
            
            
            
            
        case AxShadingOperation.BumpMap:
        {
            var pBumpMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "bumpMap", AxParameterType.ReferenceTexture);
            var pBumpMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "bumpMapFactor", AxParameterType.Float);
            
            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');
            this.vsInputType.ProvideDefinition('normal', 'attribute vec3 input_normal;');

            this.sIOType.ProvideDefinition('normal', 'varying vec3 sio_normal;');
            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            if (this.animated)
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;');
            else
                this.vsCode.ProvideDefinition('normal', '\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;');
            this.vsCode.ProvideDefinition('.normal', '\tsio_normal = normal;');
            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');

            this.psCode.ProvideDefinition('sio2_texCoords', '\tvec2 sio2_texCoords = sio_texCoords;');
            this.psCode.ProvideDefinition('sio2_normal', '\tvec3 sio2_normal = sio_normal;');
            this.psCode.ProvideDefinition('', 
                '\tvec3 bumpMapNormal = normalize(' + AxDynamicShaderWriter.TextureSampling(pBumpMap, 'sio2_texCoords') + ').xyz * ' + pBumpMapFactor + ' * 2.0 - 1.0;\r\n' +
                '\tsio2_normal += bumpMapNormal;\r\n');


            break;
        }

        
        case AxShadingOperation.BlurMap:
        {
            var pBlurMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "blurMap", AxParameterType.ReferenceTexture);
            var pBlurMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "blurMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');

            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');

            this.psCode.ProvideDefinition('sio2_texCoords', '\tvec2 sio2_texCoords = sio_texCoords;');

            this.psCode.ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this.psCode.ProvideDefinition("", 
                "\tblurSum = " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.326212, -0.405805) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.840144, -0.073580) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.695914,  0.457137) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.203345,  0.620716) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.962340, -0.194983) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.473434, -0.480026) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.519456,  0.767022) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.185461, -0.893124) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.507431,  0.064425) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2( 0.896420,  0.412458) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.321940, -0.932615) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pBlurMap, "sio2_texCoords + vec2(-0.791559, -0.597705) * " + pBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 13.0;\r\n");
           
            this.ProvideColorOperationCode(shadingOp, colorOp, "blurMapAmount", this.psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation.HBlur:
        {
            var pHBlurMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "hBlurMap", AxParameterType.ReferenceTexture);
            var pHBlurMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "hBlurMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');

            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');

            this.psCode.ProvideDefinition('sio2_texCoords', '\tvec2 sio2_texCoords = sio_texCoords;');

            this.psCode.ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this.psCode.ProvideDefinition("", 
                "\tblurSum = " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(-0.33, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(-0.66, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(-0.99, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(0.33, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(0.66, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pHBlurMap, "sio2_texCoords + vec2(0.99, 0.0) * " + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 7.0;");

            this.ProvideColorOperationCode(shadingOp, colorOp, "hBlurMapAmount", this.psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation.VBlur:
        {
            var pVBlurMap = this.ProvideParameterDefinition(this.psParams, shadingOp, "vBlurMap", AxParameterType.ReferenceTexture);
            var pVBlurMapFactor = this.ProvideParameterDefinition(this.psParams, shadingOp, "vBlurMapFactor", AxParameterType.Float);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');

            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = input_texCoords;');

            this.psCode.ProvideDefinition('sio2_texCoords', '\tvec2 sio2_texCoords = sio_texCoords;');

            this.psCode.ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this.psCode.ProvideDefinition("", 
                "\tblurSum = " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, -0.33) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, -0.66) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, -0.99) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, 0.33) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, 0.66) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter.TextureSampling(pVBlurMap, "sio2_texCoords + vec2(0.0, 0.99) * " + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 7.0;");

            this.ProvideColorOperationCode(shadingOp, colorOp, "vBlurMapAmount", this.psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation.AlignScreenQuad:
        {
            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');

            this.vsCode.ProvideDefinition('screenPos', '\tvec2 screenPos;');
            this.vsCode.ProvideDefinition('',
                    '\tscreenPos = sign(input_pos.xy);\r\n' +
                    '\tsio_pos = vec4(screenPos, 0.999, 1.0);\r\n');// + // 0.999 for zbuffer
                    //'\tsio_pos = vec4(screenPos, 0.001, 1.0);\r\n' +
                    //'\tsio_pos.y = -sio_pos.y; // remove this, instead change vertex order');

            this.vsCode.ProvideDefinition('.texCoords', '\tsio_texCoords = (screenPos + 1.0) / 2.0;');

            break;
        }
        /*



        case AxShadingOperation.ProjectTexture:
        {
                if (this.various.ProvideDefinition('projectionMap', 
                        AxString('Texture2D tProjectionMap: register(t') + AxString(this.textureBuffersInfo.count) + ');\r\n'
                        'sampler sProjectionMap: register(s' + AxString(this.textureBuffersInfo.count) + ');\r\n'))
                {
                        this.textureBuffersInfo.Add('projectionMap');
                }

                if (this.parameters.ProvideDefinition('projectionCamera', '\tmat4 projectionCamera;'))
                {
                        this.constantBuffersInfo.Get(0).Add('projectionCamera', 64);
                }

                if (this.sIOType.ProvideDefinition('projectionTexCoords', AxString('\tvec2 projectionTexCoords: TEXCOORD') + AxString(this.texCoordSlots) + ';'))
                        this.texCoordSlots++;

                this.vsCode.ProvideDefinition('projectionCoords', '\tvec4 projectionCoords;');
                this.vsCode.ProvideDefinition('', '\tprojectionCoords = mul(projectionCamera, worldPos);');
                this.vsCode.ProvideDefinition('.projectionTexCoords', '\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;');

                this.psCode.ProvideDefinition('', AxString('\t') + this.GetColorOperationCode('output.color', 'tProjectionMap.Sample(sProjectionMap, input_projectionTexCoords)', colorOp));

                break;
        }

        case AxShadingOperation.ProjectPicture:
        {
                if (this.various.ProvideDefinition('projectionMap', 
                        AxString('Texture2D tProjectionMap: register(t') + AxString(this.textureBuffersInfo.count) + ');\r\n'
                        'sampler sProjectionMap: register(s' + AxString(this.textureBuffersInfo.count) + ');\r\n'))
                {
                        this.textureBuffersInfo.Add('projectionMap');
                }

                if (this.parameters.ProvideDefinition('projectionCamera', '\tmat4 projectionCamera;'))
                {
                        this.constantBuffersInfo.Get(0).Add('projectionCamera', 64);
                }

                if (this.sIOType.ProvideDefinition('projectionTexCoords', AxString('\tvec2 projectionTexCoords: TEXCOORD') + AxString(this.texCoordSlots) + ';'))
                        this.texCoordSlots++;

                this.vsCode.ProvideDefinition('projectionCoords', '\tvec4 projectionCoords;');
                this.vsCode.ProvideDefinition('', '\tprojectionCoords = mul(projectionCamera, worldPos);');
                this.vsCode.ProvideDefinition('.projectionTexCoords', '\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;');

                this.psCode.ProvideDefinition('', 
                        AxString('\tif ((input_projectionTexCoords.x > 0.0) && (input_projectionTexCoords.x < 1.0) && (input_projectionTexCoords.y > 0.0) && (input_projectionTexCoords.y < 1.0))\r\n') +
                        '\t\t' + this.GetColorOperationCode('output.color', 'tProjectionMap.Sample(sProjectionMap, input_projectionTexCoords)', colorOp));

                break;
        }

    //*/

        case AxShadingOperation.TextureTransform:
        {
            var pTextureTransform = this.ProvideParameterDefinition(this.psParams, shadingOp, "textureTransform", AxParameterType.ReferenceTransform);

            this.vsInputType.ProvideDefinition('texCoords', 'attribute vec2 input_texCoords;');

            this.sIOType.ProvideDefinition('texCoords', 'varying vec2 sio_texCoords;');
            
            this.psCode.ProvideDefinition('sio2_texCoords', 'vec2 sio2_texCoords;');
            this.psCode.ProvideDefinition('', '\tsio2_texCoords = (' + pTextureTransform + ' * vec4(sio_texCoords, 0, 1)).xy;');
            break;
        }
        
        case AxShadingOperation.Skinning:
        {
            break;
        }

        case AxShadingOperation.Material:
        {
            this.ProvideParameterDefinition(this.vsParams, shadingOp, "ambient", AxParameterType.Color);
            this.ProvideParameterDefinition(this.vsParams, shadingOp, "diffuse", AxParameterType.Color);
            this.ProvideParameterDefinition(this.vsParams, shadingOp, "specular", AxParameterType.Color);
            this.ProvideParameterDefinition(this.vsParams, shadingOp, "gloss", AxParameterType.Float);

            this.ProvideParameterDefinition(this.psParams, shadingOp, "ambient", AxParameterType.Color);
            this.ProvideParameterDefinition(this.psParams, shadingOp, "diffuse", AxParameterType.Color);
            this.ProvideParameterDefinition(this.psParams, shadingOp, "specular", AxParameterType.Color);
            this.ProvideParameterDefinition(this.psParams, shadingOp, "gloss", AxParameterType.Float);
        }
        case AxShadingOperation.Fog:
        {
            var pFogColor = this.ProvideParameterDefinition(this.psParams, shadingOp, "fogColor", AxParameterType.Color);
            var pFogStart = this.ProvideParameterDefinition(this.vsParams, shadingOp, "fogStart", AxParameterType.Float);
            var pFogDensity = this.ProvideParameterDefinition(this.vsParams, shadingOp, "fogDensity", AxParameterType.Float);

            this.sIOType.ProvideDefinition("fogImmersion", "varying float sio_fogImmersion;");

            this.vsCode.ProvideDefinition("distance", "\tfloat distance = length(viewPosition - worldPos.xyz);");
            this.vsCode.ProvideDefinition(".fogImmersion", "\tsio_fogImmersion = clamp((distance - " + pFogStart + ") / " + pFogDensity + ", 0.0, 1.0);");

            this.psCode.ProvideDefinition("fogColorAlpha", 
                "\tvec4 fogColorAlpha = " + pFogColor + ";\r\n" +
                "\tfogColorAlpha.w = sio_fogImmersion;");

            this.ProvideColorOperationCode(shadingOp, colorOp, "fogAmount", this.psCode, "\t", "result.color", "fogColorAlpha");

            break;
        }
    }

    this.shadingOps.Add(shadingOp);
};

AxDynamicShaderWriter.prototype.GetVSSourceCode = function()
{
    if (this.numLights > 0)
    {
        this.vsParams.ProvideDefinition('',
            '#define maxLights ' + this.numLights + '\r\n' +
            'uniform vec4 lightsPosIntensity[maxLights];\r\n' +
            'uniform vec4 lightsColor[maxLights];\r\n');
        this.psParams.ProvideDefinition('',
            '#define maxLights ' + this.numLights + '\r\n' +
            'uniform vec4 lightsPosIntensity[maxLights];\r\n' +
            'uniform vec4 lightsColor[maxLights];\r\n');
    }

    return '// Generated by Axis GLSL dynamic shader writer\r\n\r\n' +

        this.vsParams.GetCode().GetContents() + '\r\n\r\n' +

        this.vsInputType.GetCode().GetContents() + '\r\n\r\n' +

        this.sIOType.GetCode().GetContents() + '\r\n\r\n' +

        this.vsCode.GetCode().GetContents() + '\r\n\tgl_Position = sio_pos;\r\n}';
};

AxDynamicShaderWriter.prototype.GetPSSourceCode = function()
{
    return '// Generated by Axis GLSL dynamic shader writer\r\n\r\n' +

        this.psParams.GetCode().GetContents() + '\r\n\r\n' +

        this.sIOType.GetCode().GetContents() + '\r\n\r\n' +

        this.psOutputType.GetCode().GetContents() + '};\r\n\r\n' +

        this.psCode.GetCode().GetContents() + 
            '\r\n' +
            '\tgl_FragColor = result.color;\r\n' +
            '\tgl_FragColor.w = 1.0;\r\n}';
};


// Private methods


/*
 * Private
 * Provides a parameter definition for the shader, giving it a unique names, by adding a number, if required, to the desired name, based on the number of the already existing shading layers with the given shading operation
 * @param {AxDynamicShaderBlockDefinitions} definitions The shader block definitions for which the color operation code shoud be generated
 * @param {AxShadingOperation} shadingOp The shading operation for which to provide the parameter
 * @param {String} desiredName The desired name for the parameter. If a parameter with the given desired name exists, a unique name will be generated, by concatenating a number to the given one
 * @param {AxParameterType} type The type of the parameter which is to be provided
 * @param {!Integer} elementsCount The number of elements of the parameter. For values other than 1, the parameter is automatically assumed to be an array. If omitted, a value of 1 is assumed.
 * @param {!Boolean} isArray Denotes whether the parameter is an array. If omitted, a value of false is assumed.
 * @return {String} The name of the provided parameter
 */
AxDynamicShaderWriter.prototype.ProvideParameterDefinition = function(definitions, shadingOp, desiredName, type, elementsCount, isArray)
{
    if (AxUtils.IsUndefinedOrNull(elementsCount))
        elementsCount = 1;

    if (AxUtils.IsUndefinedOrNull(isArray))
        isArray = false;
    
    var shadingOpCount = this.shadingOps.Count(shadingOp);
    var name = shadingOpCount === 0 ? desiredName : (desiredName + shadingOpCount);
    isArray = isArray || (elementsCount > 1);
    
    var typeName;
    switch (type)
    {
        case AxParameterType.Int: 
        { 
            typeName = "int";
            break; 
        }
        case AxParameterType.Float:
        { 
            typeName = "float";
            break; 
        }
        case AxParameterType.Bool:
        { 
            typeName = "bool";
            break; 
        }
        case AxParameterType.Color:
        { 
            typeName = "vec4";
            break; 
        }
        case AxParameterType.ReferenceVector3:
        { 
            typeName = "vec3";
            break; 
        }
        case AxParameterType.ReferenceCamera:
        { 
            typeName = "mat4";
            break; 
        }
        case AxParameterType.ReferenceTransform:
        { 
            typeName = "mat4";
            break; 
        }
        case AxParameterType.ReferenceTexture:
        { 
            typeName = "sampler2D";
            break; 
        }
    }
    
    definitions.ProvideDefinition(name, 'uniform' + ' ' + typeName + ' ' + name + ';');
    
    return name;
};

/**
 * Private
 * Creates the proper source code for a color operation. According to the color operation, a "factor" shader property is generated. The code is added to the given AxDynamicShaderBlockDefinitions.
 * @param {AxShadingOperation} shadingOp The shading operation for which the color operation code is to be generated. It is used to give the proper name to the factor
 * @param {AxColorOperation} colorOp The color operation which is to be performed
 * @param {String} desiredFactorName The desired name for the factor of the operation. The factor is a value which generally detrmines the effective amount of the sourceArg onto the destArg
 * @param {AxDynamicShaderBlockDefinitions} definitions The shader block definitions for which the color operation code shoud be generated
 * @param {String} linePrefix A prefix for each line of code, generated by the method. Typically used to add tabs
 * @param {String} destArg The destination argument, which is to hold the result of the operation
 * @param {String} sourceArg The souce argument, which is to be applied on the destination
 */
AxDynamicShaderWriter.prototype.ProvideColorOperationCode = function(shadingOp, colorOp, desiredFactorName, definitions, linePrefix, destArg, sourceArg)
{
    var paramsDefinitions = definitions === this.psCode ? this.psParams : this.vsParams;
    
    switch (colorOp)
    {
        case AxColorOperation.None:
            break;

        case AxColorOperation.Set:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " = " + sourceArg + " * " + factor + ";");
            
            break;
        }

        case AxColorOperation.Blend:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " = mix(" + destArg + ", (" + sourceArg + "), " + factor + ");");
            
            break;
        }
        
        case AxColorOperation.Add:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " += (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation.Subtract:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " -= (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation.Modulate:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " *= (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation.Lighter:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " = max(" + destArg + ", " + sourceArg + " * " + factor + ");");
            
            break;
        }

        case AxColorOperation.Darker:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("", linePrefix + destArg + " = min(" + destArg + ", " + sourceArg + " * " + factor + ");");
            
            break;
        }

        case AxColorOperation.AlphaBlend:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions.ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " = mix(" + destArg + ", colorOpArg, colorOpArg.w * " + factor + ");");
            
            break;
        }

        case AxColorOperation.AlphaAdd:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions.ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " += " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        case AxColorOperation.AlphaSubtract:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions.ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " -= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        case AxColorOperation.AlphaModulate:
        {
            var factor = this.ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType.Float);
            definitions.ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions.ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " *= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        default:
        {
            definitions.ProvideDefinition("", linePrefix + destArg + " = 0;");

            break;
        }    
    }
};

/*
 * Converts from a string from a Axis property name format to GLSL parameter name (code variable) format
 * The convention for a GLSL property name is that it starts with a lower case, each next word of the name begins with an upper case and there are no spaces between words and numbers.
 * It is used to directly convert a Axis property name to a GLSL paramter name (that is, a GLSL code variable)
 * @param {AxString} propertyName Name of a axis property which is to be converted to a valid GLSL code variable name
 * @return {AxString} The propertyName in the format of a valid GLSL parameter
 */
AxDynamicShaderWriter.PropertyNameToParameterName = function(propertyName)
{
    var result = new AxString();

    var isFirstCharacter = true;
    var character;
    var lastCharacter = new AxString();
    for (var i = 0; i < propertyName.length; i++)
    {
        character = new AxString(propertyName.contents[i]);

        if (!character.Equals(' '))
        {
            if (isFirstCharacter)
            {
                result.Insert((new AxString(character)).ToLower());
                isFirstCharacter = false;
            }
            else
            {
                if (lastCharacter.Equals(' '))
                    result.Insert((new AxString(character)).ToUpper());
                else
                    result.Insert(character);
            }
        }

        lastCharacter = character;
    }

    return new AxString(result);
};

AxDynamicShaderWriter.CorrectTexCoords = function(texCoords, texCoords1)
{
    if (AxUtils.IsUndefinedOrNull(texCoords1))
    {
        return texCoords;
        //return "(" + texCoords + ") * vec2(1.0, -1.0) + vec2(0.0, 1.0)";
    }
    else
    {
        return "vec2(" + texCoords + ", " + texCoords1 + ")";
        //return "vec2(" + texCoords + ", 1.0 - " + texCoords1 + ")";
    }
};

AxDynamicShaderWriter.TextureSampling = function(texture, texCoords, texCoords1)
{
    return 'texture2D(' + texture + ', ' + AxDynamicShaderWriter.CorrectTexCoords(texCoords, texCoords1) + ')';
};
