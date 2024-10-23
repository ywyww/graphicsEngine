#include "Scene.h"


namespace SceneNamespace 
{
    Scene::Scene()
    {
        linesName = "lines";
    }

    void Scene::addLine(Line* line)
    {
        NodeGroup<Line> lineGrp;
        lineGrp.node = line;
        lineGrp.name = "line1";

        lines.push_back(lineGrp);
    }

    Lines& Scene::getLines()
    {
        return lines;
    }

    void Scene::drawLines()
    {
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i].node->draw();
        }
    }
}
