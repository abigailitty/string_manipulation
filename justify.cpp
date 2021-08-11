//Abby Itty
//DataStructures HW1 : justify text
//sorry if a lot of my comments are just note to self! i'm trying to remind myself of concepts so i can learn them better :)

#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>


//right or left justify
void rightJustify(std::vector<std::string> &text, const int lineLength , std::ofstream &outfile, std::string side)
{
    //top line
    std::string ends(lineLength + 4,'-');
    outfile << ends << std::endl;

    //middle section
    //figuring out how many words to a line
    unsigned int currentLength = 0;
    unsigned int i = 0;
    std::string line;
    while(i < text.size())
    {
        while((currentLength + text[i].size()) < lineLength)
        {
            line += text[i];
            currentLength += text[i].size();
            i++;
            if((currentLength + 1 + text[i].size()) < lineLength && i < text.size())
            {
                line += ' ';
                currentLength += 1;
            }
        }
        if(side == "right")
            outfile << "| " << std::string(lineLength-line.size(),' ') << line << " |" << std::endl;
        else
            outfile << "| " << line << std::string(lineLength-line.size(),' ') << " |" << std::endl;
        currentLength = 0;
        line.clear();
    }

    //bottom line
    outfile << ends << std::endl;
    outfile.close();
}

//fully justify
void centerJustify(std::vector<std::string> &text, const int lineLength, std::ofstream &outfile)
{
    //top line
    std::string ends(lineLength + 4,'-');
    outfile << ends << std::endl;

    //middle section
    //figuring out how many words to a line
    unsigned int i = 0;
    while(i < text.size())
    {
        int currentLength = 0;
        std::string line;
        while((currentLength + text[i].size()) < lineLength)
        {
            line += text[i];
            currentLength += text[i].size();
            i++;
            if((currentLength + 1 + text[i].size()) < lineLength && i < text.size())
            {
                line += ' ';
                currentLength += 1;
            }
        }

        //moving around spaces to be evenly distributed
        int extraSpaces = lineLength-line.size();

        //find all spaces in line
        std::vector<int> spacePos;
        for(unsigned int j = 0; j < line.size(); j++)
            if(line[j] == ' ')
                spacePos.push_back(j);

        //check if the line is only one word -left justify if it is
        if(spacePos.size() == 0)
            outfile << "| " <<  line << std::string(lineLength - line.size(), ' ') << " |" << std::endl;
        else
        {

            std::string spaceString(int(extraSpaces / spacePos.size()), ' ');

            for (unsigned int k = 0; k < spacePos.size(); k++)
            {
                line.insert(spacePos[k], spaceString); //inserted a string, so every position is shoved over after that
                for (unsigned int j = k; j < spacePos.size(); j++) //start at k
                    spacePos[j] += spaceString.size();
                extraSpaces -= spaceString.size();
            }

            int l = 0;
            while (extraSpaces > 0 && l < spacePos.size())
            {
                line.insert(spacePos[l], " ");
                extraSpaces--;
                for (int j = l; j < spacePos.size(); j++) //again, move everything over
                    spacePos[j] += 1;
                l++;
            }

            outfile << "| " << line << " |" << std::endl;
        }
    }

    //bottom line
    outfile << ends << std::endl;
    outfile.close();
}

int main(int argc, char* argv[])
{
    //error checking inputs

    //4 args?
    if(argc != 5)
    {
        std::cerr << "Not five arguments" << std::endl;
        return 1;
    }



    //is line length a # / positive line length?
    for(unsigned int i = 0; i < std::string(argv[3]).size(); i++)
        if(!isdigit(std::string(argv[3])[i]))
        {
            std::cerr << "Third argument was not an integer." << std::endl;
            return 1;
        }
    if(atoi(argv[3]) <= 0)
    {
        std::cerr << "Third argument was not positive." << std::endl;
        return 1;
    }

    //user picked a valid option?
    if(std::string(argv[4]) != "flush_left" && std::string(argv[4]) != "flush_right" && std::string(argv[4]) != "full_justify")
    {
        std::cerr << "Fourth argument is not a choice. Please type only flush_right, flush_left, or full_justify next time." << std::endl;
        return 1;
    }

    //infile open?
    std::ifstream infile(argv[1]);
    if(!infile.good())
    {
        std::cerr << "Can't open " << std::string(argv[1]) << " to read." << std::endl;
        return 1;
    }

    //read in blurb to justify
    std::vector<std::string> text;
    std::string s;

    while(infile >> s)
    {
        text.push_back(s);
    }

    infile.close();

    //outfile stream -> open after infile is closed to avoid it getting trashed
    std::ofstream outfile(argv[2]);
    if(!outfile.good())
    {
        std::cerr << "Can't open " << std::string(argv[2]) << " to read." << std::endl;
        return 1;
    }

    //send to separate functions
    std::string side;
    if(std::string(argv[4]) == "flush_right")
    {
        side = "right";
        rightJustify(text, atoi(argv[3]), outfile, side);
    }
    else if(std::string(argv[4]) == "flush_left")
    {
        side = "left";
        rightJustify(text, atoi(argv[3]), outfile, side);
    }
    else //arg4 is fully_justify
        centerJustify(text, atoi(argv[3]), outfile);

    return 0;
}