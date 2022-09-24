#include "fbx_loader.h"
#include "loaders/stb_image.h"

Node::Node(int level, FILE *file)
{
    this->level = level;
    this->file = file;
}

void Node::process()
{
    readHead();
    if (name == 0)
    {
        bIsZero = true;
        return;
    }
    else
    {
        if (doSkip(name))
        {
            fseek(file, endOffset, SEEK_SET);
            bMarkedToRemove = true;
            return;
        }

        if (numProperties > 0)
            readValues();

        int p = ftell(file);
        while (p < endOffset)
        {
            Node *child = new Node(level + 1, file);
            if (!child)
                break;

            child->process();

            if (child->bIsZero)
            {
                delete child;
                break;
            }
            else if (child->bMarkedToRemove)
            {
                delete child;
                p = ftell(file);
                continue;
            }
            else
            {
                p = ftell(file);
                children.push_back(child);
            }
        }
    }
}

void Node::readHead()
{
    NodeRecordHeader header;
    fread(&header, sizeof(NodeRecordHeader), 1, file);
    unsigned char nameLength;
    if (feof(file))
        return;

    fread(&nameLength, 1, 1, file);
    if (nameLength == 0)
        return;

    endOffset = header.endOffset;
    numProperties = header.numProperties;
    propertyListLen = header.propertyListLen;

    name = (char *)malloc(nameLength + 1);
    fread(name, nameLength, 1, file);
    name[nameLength] = 0;
    // printf("Entry %s, offset: %i, num: %i, listLen: %i\n", name, header.endOffset, header.numProperties, header.propertyListLen);
}

void Node::readValues()
{
    if (!propertyListLen)
        return;

    int rawLength = 0, p = 0, b = 0, iValue;
    unsigned int stringLength = 0, arrayLength, encoding, compressedLength, destLen, sourceLen, dataLength;
    double dValue;
    long int longValue;

    char *cData;
    int *iData;
    double *dData;
    long long *lData;

    char *property = (char *)malloc(propertyListLen + 1);
    if (!property)
        return;

    fread(property, propertyListLen, 1, file);

    for (int i = 0; i < numProperties; i++)
    {
        switch (property[p])
        {
        case 'S':
            stringLength = *((int *)&property[p + 1]);
            p += 5;
            if (stringLength)
            {
                cData = (char *)malloc(stringLength + 1);
                memcpy(cData, property + p, stringLength);
                cData[stringLength] = 0;
                bindedData.push_back(NodeDataBinding({'S', (void *)cData, stringLength}));

                p += stringLength;
            }
            break;

        case 'I':
            iValue = *((int *)&property[p + 1]);
            p += 5;
            iData = (int *)malloc(sizeof(int));
            *iData = dValue;
            bindedData.push_back(NodeDataBinding({'I', (void *)iData, 1}));
            break;

        case 'R':
            rawLength = *((int *)&property[p + 1]);
            p += 5 + rawLength;
            bindedData.push_back(NodeDataBinding({'R', (void *)0, 0}));
            break;

        case 'D':
            dValue = *((double *)&property[p + 1]);
            p += 9;
            dData = (double *)malloc(sizeof(double));
            *dData = dValue;
            bindedData.push_back(NodeDataBinding({'D', (void *)dData, 1}));
            break;

        case 'L':
            longValue = *((long int *)&property[p + 1]);
            p += 9;
            lData = (long long *)malloc(sizeof(long long));
            *lData = longValue;
            bindedData.push_back(NodeDataBinding({'L', (void *)lData, 1}));
            break;

        case 'd':
            arrayLength = *((unsigned int *)&property[p + 1]);
            encoding = *((unsigned int *)&property[p + 5]);
            compressedLength = *((unsigned int *)&property[p + 9]);
            destLen = arrayLength * sizeof(double);
            sourceLen = compressedLength;

            p += 13;
            dData = (double *)malloc(destLen);
            dataLength = arrayLength;

            if (encoding)
                b = stbi_zlib_decode_buffer((char *)dData, destLen, &property[p], compressedLength);
            else
                memcpy(dData, &property[p], destLen);

            bindedData.push_back(NodeDataBinding({'d', (void *)dData, arrayLength}));
            break;

        case 'i':
            arrayLength = *((unsigned int *)&property[p + 1]);
            encoding = *((unsigned int *)&property[p + 5]);
            compressedLength = *((unsigned int *)&property[p + 9]);
            destLen = arrayLength * sizeof(int);
            sourceLen = compressedLength;

            p += 13;
            iData = (int *)malloc(destLen);
            dataLength = arrayLength;

            if (encoding)
                b = stbi_zlib_decode_buffer((char *)iData, destLen, &property[p], compressedLength);
            else
                memcpy(iData, &property[p], destLen);

            bindedData.push_back(NodeDataBinding({'i', (void *)iData, arrayLength}));
            break;

        default:
            printf("Unknown type %c\n", property[p]);
            return;
        }
    }
    free(property);
}

void Node::print()
{
    for (int i = 0; i < level; i++)
        printf(" ");

    printf("%s\n", name);
    for (auto it = bindedData.begin(); it != bindedData.end(); it++)
    {
        for (int i = 0; i < level; i++)
            printf(" ");
        printf("-");

        if ((*it).type == 'S')
            printf("%s", (char *)(*it).data);
        if ((*it).type == 'I')
            printf("%i", (int)*(int *)(*it).data);
        if ((*it).type == 'D')
            printf("%f", (float)*(double *)(*it).data);
        if ((*it).type == 'L')
            printf("%i", (int)*(long long *)(*it).data);
        if ((*it).type == 'd')
            printf("Double array");
        if ((*it).type == 'i')
            printf("Long long array");

        printf("(%c)\n", (*it).type);
    }

    for (auto it = children.begin(); it != children.end(); it++)
    {
        (*it)->print();
    }
}

bool Node::isName(const char *name)
{
    return strcmp(this->name, name) == 0;
}

Node *Node::findNode(const char *name)
{
    for (auto it = children.begin(); it != children.end(); it++)
        if ((*it)->isName(name))
            return (*it);
    return nullptr;
}

bool Node::doSkip(const char *name)
{
    return (
        (strcmp(name, "Properties70") == 0) ||
        (strcmp(name, "FBXHeaderExtension") == 0) ||
        (strcmp(name, "Connections") == 0) ||
        (strcmp(name, "Takes") == 0) ||
        (strcmp(name, "Document") == 0) ||
        (strcmp(name, "Definitions") == 0));
}

bool FBXLoader::load(std::string meshPath)
{
    printf("Load 3d file %s\n", meshPath.c_str());
    fopen_s(&file, meshPath.c_str(), "rb");

    char buff[21];
    fread(buff, 21, 1, file);
    if (std::strcmp(buff, "Kaydara FBX Binary  ") != 0)
    {
        printf("Loading %s failed\n", meshPath.c_str());
        return false;
    }

    fread(buff, 2, 1, file);
    if (buff[0] != 0x1A || buff[1] != 0x00)
    {
        printf("Loading %s failed\n", meshPath.c_str());
        return false;
    }

    unsigned int version;
    fread(&version, 4, 1, file);
    if (version != 7400)
    {
        printf("Only supported version of FBX format is 7.4\n");
        return false;
    }

    while (1)
    {
        Node *node = new Node(0, file);
        node->process();
        if (node->bIsZero)
            break;
        if (node->bMarkedToRemove)
            delete node;
        else
            nodes.push_back(node);
    }

    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->print();
    }

    root = new MeshDescriptor();
    current = root;

    sendToMeshDescriptor(root);

    return true;
}

void FBXLoader::print()
{
    for (auto it = nodes.begin(); it != nodes.end(); it++)
        (*it)->print();
}

void FBXLoader::sendToMeshDescriptor(MeshDescriptor *meshDescriptor)
{
    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        Node *node = (*it);
        if (node->isName("Objects"))
        {
            Node *geometry = node->findNode("Geometry");
            if (geometry)
            {
                Node *vertices = geometry->findNode("Vertices");
                Node *polygonVertexIndex = geometry->findNode("PolygonVertexIndex");
                Node *elementUVs = geometry->findNode("LayerElementUV");
                Node *elementNormals = geometry->findNode("LayerElementNormal");

                if (vertices && vertices->bindedData.size() > 0)
                {
                    auto nodeData = vertices->bindedData.at(0);
                    current->provideVertex((double *)nodeData.data, nodeData.numElements);
                }
                if (polygonVertexIndex && polygonVertexIndex->bindedData.size() > 0)
                {
                    auto nodeData = polygonVertexIndex->bindedData.at(0);
                    current->providePolygonIndexes((int *)nodeData.data, nodeData.numElements);
                }
                if (elementUVs)
                {
                    Node *UVs = elementUVs->findNode("UV");
                    Node *UVIndexes = elementUVs->findNode("UVIndex");
                    if (UVs && UVs->bindedData.size() > 0)
                    {
                        auto nodeData = UVs->bindedData.at(0);
                        current->provideUVData((double *)nodeData.data, nodeData.numElements);
                    }
                    if (UVIndexes && UVIndexes->bindedData.size() > 0)
                    {
                        auto nodeData = UVIndexes->bindedData.at(0);
                        current->provideUVIndexes((int *)nodeData.data, nodeData.numElements);
                    }
                }
                if (elementNormals)
                {
                    Node *normals = elementNormals->findNode("Normals");
                    if (normals && normals->bindedData.size())
                    {
                        auto nodeData = normals->bindedData.at(0);
                        current->provideNormals((double *)nodeData.data, nodeData.numElements);
                    }
                }
            }
        }
    }
}
