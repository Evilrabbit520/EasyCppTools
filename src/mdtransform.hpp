/**
 * @file mdtransform.hpp
 * @author Evilrabbit (evilrabbit520@gmail.com)
 * @brief Markdown parser (HTML implementation version).
 * @version 1.0.0
 * @date 2023-05-10
 *
 * @copyright Copyright (c) 2021-2023 Evilrabbit. All rights reserved.
 *
 */
#ifndef EASYCPPTOOLS_SRC_MDTOHTML_HPP__
#define EASYCPPTOOLS_SRC_MDTOHTML_HPP__

#include <cstdlib>
#include <fstream>
#include <vector>
#include <cstring>
#include <utility>
#include <string>
#include <cctype>
#include <cstdio>
using namespace std;

#define maxLength 10000

namespace ECT
{
    // 词法关键字枚举
    enum
    {
        nul = 0,
        paragraph = 1,
        href = 2,
        ul = 3,
        ol = 4,
        li = 5,
        em = 6,
        strong = 7,
        hr = 8,
        br = 9,
        image = 10,
        quote = 11,
        h1 = 12,
        h2 = 13,
        h3 = 14,
        h4 = 15,
        h5 = 16,
        h6 = 17,
        blockcode = 18,
        code = 19
    };
    // HTML 前置标签
    const string frontTag[] =
        {
            "", "<p>", "", "<ul>", "<ol>", "<li>", "<em>", "<strong>",
            "<hr color=#CCCCCC size=1 />", "<br />",
            "", "<blockquote>",
            "<h1 ", "<h2 ", "<h3 ", "<h4 ", "<h5 ", "<h6 ", // 右边的尖括号预留给添加其他的标签属性
            "<pre><code>", "<code>"};
    // HTML 后置标签
    const string backTag[] =
        {
            "", "</p>", "", "</ul>", "</ol>", "</li>", "</em>", "</strong>",
            "", "", "", "</blockquote>",
            "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>",
            "</code></pre>", "</code>"};
    typedef struct Cnode
    {
        vector<Cnode *> ch;
        string heading;
        string tag;
        Cnode(const string &hd) : heading(hd) {}
    } Cnode;

    typedef struct node
    {
        int type; // 节点代表的类型
        vector<node *> ch;
        string elem[3]; // 用来存放三个重要的属性, elem[0] 保存了要显示的内容
                        // elem[1] 保存了链接, elem[2] 则保存了 title
        node(int _type) : type(_type) {}
    } node;

    class MarkdownToHTML
    {
    private:
        node *root, *now;
        Cnode *Croot;
        string content, TOC;
        int cntTag = 0;
        char s[maxLength];

        // 判断是否为标题
        inline bool isHeading(node *v)
        {
            return (v->type >= h1 && v->type <= h6);
        }
        // 判断是否为图片
        inline bool isImage(node *v)
        {
            return (v->type == image);
        }
        // 判断是否为超链接
        inline bool isHref(node *v)
        {
            return (v->type == href);
        }
        // 递归销毁数节点
        template <typename T>
        void destroy(T *v)
        {
            for (int i = 0; i < (int)v->ch.size(); i++)
            {
                destroy(v->ch[i]);
            }
            delete v;
        }

        void Cdfs(Cnode *v, string index)
        {
            TOC += "<li>\n";
            TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
            int n = (int)v->ch.size();
            if (n)
            {
                TOC += "<ul>\n";
                for (int i = 0; i < n; i++)
                {
                    Cdfs(v->ch[i], index + to_string(i + 1) + ".");
                }
                TOC += "</ul>\n";
            }
            TOC += "</li>\n";
        }

        void Cins(Cnode *v, int x, const string &hd, int tag)
        {
            int n = (int)v->ch.size();
            if (x == 1)
            {
                v->ch.push_back(new Cnode(hd));
                v->ch.back()->tag = "tag" + to_string(tag);
                return;
            }

            if (!n || v->ch.back()->heading.empty())
                v->ch.push_back(new Cnode(""));
            Cins(v->ch.back(), x - 1, hd, tag);
        }

        void dfs(node *v)
        {
            if (v->type == paragraph && v->elem[0].empty() && v->ch.empty())
                return;

            content += frontTag[v->type];
            bool flag = true;

            // 处理标题, 支持用目录进行跳转
            if (isHeading(v))
            {
                content += "id=\"" + v->elem[0] + "\">";
                flag = false;
            }

            // 处理超链接
            if (isHref(v))
            {
                content += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
                flag = false;
            }

            // 处理图片
            if (isImage(v))
            {
                content += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\" />";
                flag = false;
            }

            // 如果上面三者都不是, 则直接添加内容
            if (flag)
            {
                content += v->elem[0];
                flag = false;
            }

            // 递归遍历所有
            for (int i = 0; i < (int)v->ch.size(); i++)
                dfs(v->ch[i]);

            // 拼接为结束标签
            content += backTag[v->type];
        }

        // 判断是否换行
        inline bool isCutline(char *src)
        {
            int cnt = 0;
            char *ptr = src;
            while (*ptr)
            {
                // 如果不是 空格、tab、- 或 *，那么则不需要换行
                if (*ptr != ' ' && *ptr != '\t' && *ptr != '-')
                    return false;
                if (*ptr == '-')
                    cnt++;
                ptr++;
            }
            // 如果出现 --- 则需要增加一个分割线, 这时需要换行
            return (cnt >= 3);
        }

        // 生成段落
        inline void mkpara(node *v)
        {
            if (v->ch.size() == 1u && v->ch.back()->type == paragraph)
                return;
            if (v->type == paragraph)
                return;
            if (v->type == nul)
            {
                v->type = paragraph;
                return;
            }
            node *x = new node(paragraph);
            x->ch = v->ch;
            v->ch.clear();
            v->ch.push_back(x);
        }

        // 开始解析一行中开始的空格和 Tab
        // src: 源串
        // 返回值: 由空格数和有内容处的 char* 指针组成的 std::pair
        inline pair<int, char *> start(char *src)
        {
            // 如果该行内容为空，则直接返回
            if ((int)strlen(src) == 0)
                return make_pair(0, nullptr);
            // 统计空格键和 Tab 键的个数
            int cntspace = 0, cnttab = 0;
            // 从该行的第一个字符读其, 统计空格键和 Tab 键,
            // 当遇到不是空格和 Tab 时，立即停止
            for (int i = 0; src[i] != '\0'; i++)
            {
                if (src[i] == ' ')
                    cntspace++;
                else if (src[i] == '\t')
                    cnttab++;
                // 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
                // 其中, 一个 Tab = 四个空格
                return make_pair(cnttab + cntspace / 4, src + i);
            }
            return make_pair(0, nullptr);
        }

        // 判断当前行的类型
        // src: 源串
        // 返回值: 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
        inline pair<int, char *> JudgeType(char *src)
        {
            char *ptr = src;

            // 跳过 `#`
            while (*ptr == '#')
                ptr++;

            // 如果出现空格, 则说明是 `<h>` 标签
            if (ptr - src > 0 && *ptr == ' ')
                return make_pair(ptr - src + h1 - 1, ptr + 1);

            // 重置分析位置
            ptr = src;

            // 如果出现 ``` 则说明是代码块
            if (strncmp(ptr, "```", 3) == 0)
                return make_pair(blockcode, ptr + 3);

            // 如果出现 * + -, 并且他们的下一个字符为空格，则说明是列表
            if (strncmp(ptr, "- ", 2) == 0)
                return make_pair(ul, ptr + 1);

            // 如果出现 > 且下一个字符为空格，则说明是引用
            if (*ptr == '>' && (ptr[1] == ' '))
                return make_pair(quote, ptr + 1);

            // 如果出现的是数字, 且下一个字符是 . 则说明是是有序列表
            char *ptr1 = ptr;
            while (*ptr1 && (isdigit(*ptr1)))
                ptr1++;
            if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ')
                return make_pair(ol, ptr1 + 1);

            // 否则，就是普通段落
            return make_pair(paragraph, ptr);
        }

        // 给定树的深度寻找节点
        // depth: 树的深度
        // 返回值: 找到的节点指针
        inline node *findnode(int depth)
        {
            node *ptr = root;
            while (!ptr->ch.empty() && depth != 0)
            {
                ptr = ptr->ch.back();
                if (ptr->type == li)
                    depth--;
            }
            return ptr;
        }

        // 向指定的节点中插入要处理的串
        // v: 节点
        // src: 要处理的串
        void insert(node *v, const string &src)
        {
            int n = (int)src.size();
            bool incode = false,
                 inem = false,
                 instrong = false,
                 inautolink = false;
            v->ch.push_back(new node(nul));

            for (int i = 0; i < n; i++)
            {
                char ch = src[i];
                if (ch == '\\')
                {
                    ch = src[++i];
                    v->ch.back()->elem[0] += string(1, ch);
                    continue;
                }

                // 处理行内代码
                if (ch == '`' && !inautolink)
                {
                    incode ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(code));
                    incode = !incode;
                    continue;
                }

                // 处理加粗
                if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink)
                {
                    ++i;
                    instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
                    instrong = !instrong;
                    continue;
                }
                if (ch == '_' && !incode && !instrong && !inautolink)
                {
                    inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
                    inem = !inem;
                    continue;
                }

                // 处理图片
                if (ch == '!' && (i < n - 1 && src[i + 1] == '[') && !incode && !instrong && !inem && !inautolink)
                {
                    v->ch.push_back(new node(image));
                    for (i += 2; i < n - 1 && src[i] != ']'; i++)
                        v->ch.back()->elem[0] += string(1, src[i]);
                    i++;
                    for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                        v->ch.back()->elem[1] += string(1, src[i]);
                    if (src[i] != ')')
                        for (i++; i < n - 1 && src[i] != ')'; i++)
                            if (src[i] != '"')
                                v->ch.back()->elem[2] += string(1, src[i]);
                    v->ch.push_back(new node(nul));
                    continue;
                }

                // 处理超链接
                if (ch == '[' && !incode && !instrong && !inem && !inautolink)
                {
                    v->ch.push_back(new node(href));
                    for (i++; i < n - 1 && src[i] != ']'; i++)
                        v->ch.back()->elem[0] += string(1, src[i]);
                    i++;
                    for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                        v->ch.back()->elem[1] += string(1, src[i]);
                    if (src[i] != ')')
                        for (i++; i < n - 1 && src[i] != ')'; i++)
                            if (src[i] != '"')
                                v->ch.back()->elem[2] += string(1, src[i]);
                    v->ch.push_back(new node(nul));
                    continue;
                }

                v->ch.back()->elem[0] += string(1, ch);
                if (inautolink)
                    v->ch.back()->elem[1] += string(1, ch);
            }
            if (src.size() >= 2)
                if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ')
                    v->ch.push_back(new node(br));
        }

    public:
        // 构造函数
        MarkdownToHTML(const std::string &filename)
        {
            Croot = new Cnode("");
            root = new node(nul);
            now = root;

            std::ifstream fin(filename);

            bool newpara = false;
            bool inblock = false;
            while (!fin.eof())
            {
                // 从文件中获取一行
                fin.getline(s, maxLength);

                // 处理不在代码块且需要换行的情况
                if (!inblock && isCutline(s))
                {
                    now = root;
                    now->ch.push_back(new node(hr));
                    newpara = false;
                    continue;
                }

                // std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
                // 计算一行中开始的空格和 Tab 数
                std::pair<int, char *> ps = start(s);

                // 如果没有位于代码块中, 且没有统计到空格和 Tab, 则直接读取下一行
                if (!inblock && ps.second == nullptr)
                {
                    now = root;
                    newpara = true;
                    continue;
                }

                // 分析该行文本的类型
                std::pair<int, char *> tj = JudgeType(ps.second);

                // 如果是代码块类型
                if (tj.first == blockcode)
                {
                    // 如果位于代码块中, 则 push 一个空类型的节点
                    inblock ? now->ch.push_back(new node(nul)) : now->ch.push_back(new node(blockcode));
                    inblock = !inblock;
                    continue;
                }

                // 如果在代码块中, 直接将内容拼接到当前节点中
                if (inblock)
                {
                    now->ch.back()->elem[0] += string(s) + '\n';
                    continue;
                }

                // 如果是普通段落
                if (tj.first == paragraph)
                {
                    if (now == root)
                    {
                        now = findnode(ps.first);
                        now->ch.push_back(new node(paragraph));
                        now = now->ch.back();
                    }
                    bool flag = false;
                    if (newpara && !now->ch.empty())
                    {
                        node *ptr = nullptr;
                        for (auto i : now->ch)
                        {
                            if (i->type == nul)
                                ptr = i;
                        }
                        if (ptr != nullptr)
                            mkpara(ptr);
                        flag = true;
                    }
                    if (flag)
                    {
                        now->ch.push_back(new node(paragraph));
                        now = now->ch.back();
                    }
                    now->ch.push_back(new node(nul));
                    insert(now->ch.back(), string(tj.second));
                    newpara = false;
                    continue;
                }

                now = findnode(ps.first);

                // 如果是标题行, 则向其标签中插入属性 tag
                if (tj.first >= h1 && tj.first <= h6)
                {
                    now->ch.push_back(new node(tj.first));
                    now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
                    insert(now->ch.back(), string(tj.second));
                    Cins(Croot, tj.first - h1 + 1, string(tj.second), cntTag);
                }

                // 如果是无序列表
                if (tj.first == ul)
                {
                    if (now->ch.empty() || now->ch.back()->type != ul)
                    {
                        now->ch.push_back(new node(ul));
                    }
                    now = now->ch.back();
                    bool flag = false;
                    if (newpara && !now->ch.empty())
                    {
                        node *ptr = nullptr;
                        for (auto i : now->ch)
                        {
                            if (i->type == li)
                                ptr = i;
                        }
                        if (ptr != nullptr)
                            mkpara(ptr);
                        flag = true;
                    }
                    now->ch.push_back(new node(li));
                    now = now->ch.back();
                    if (flag)
                    {
                        now->ch.push_back(new node(paragraph));
                        now = now->ch.back();
                    }
                    insert(now, string(tj.second));
                }

                // 如果是有序列表
                if (tj.first == ol)
                {
                    if (now->ch.empty() || now->ch.back()->type != ol)
                    {
                        now->ch.push_back(new node(ol));
                    }
                    now = now->ch.back();
                    bool flag = false;
                    if (newpara && !now->ch.empty())
                    {
                        node *ptr = nullptr;
                        for (auto i : now->ch)
                        {
                            if (i->type == li)
                                ptr = i;
                        }
                        if (ptr != nullptr)
                            mkpara(ptr);
                        flag = true;
                    }
                    now->ch.push_back(new node(li));
                    now = now->ch.back();
                    if (flag)
                    {
                        now->ch.push_back(new node(paragraph));
                        now = now->ch.back();
                    }
                    insert(now, string(tj.second));
                }

                // 如果是引用
                if (tj.first == quote)
                {
                    if (now->ch.empty() || now->ch.back()->type != quote)
                    {
                        now->ch.push_back(new node(quote));
                    }
                    now = now->ch.back();
                    if (newpara || now->ch.empty())
                        now->ch.push_back(new node(paragraph));
                    insert(now->ch.back(), string(tj.second));
                }

                newpara = false;
            }

            // 文件读取分析完毕
            fin.close();

            // 深入优先遍历整个语法树
            dfs(root);

            // 构造目录
            TOC += "<ul>";
            for (int i = 0; i < (int)Croot->ch.size(); i++)
                Cdfs(Croot->ch[i], to_string(i + 1) + ".");
            TOC += "</ul>";
        }

        // 获得 Markdown 目录
        string getTableOfContents() { return TOC; }
        // 获得 Markdown 内容
        string getContents() { return content; }

        // 析构函数
        ~MarkdownToHTML()
        {
            destroy<node>(root);
            destroy<Cnode>(Croot);
        }
    };
}
#endif // EASYCPPTOOLS_SRC_MDTOHTML_HPP__
