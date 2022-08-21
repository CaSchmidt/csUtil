#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>

#include <QtCreator/HighlightingItemDelegate.h>

#include <cs/Qt/AbstractTreeItem.h>
#include <cs/Qt/TreeModel.h>

#include "ProxyDelegate.h"

////// Tree Structure ////////////////////////////////////////////////////////

class TreeRoot : public cs::AbstractTreeItem {
public:
  TreeRoot(cs::AbstractTreeItem *parent = nullptr)
    : cs::AbstractTreeItem(parent)
  {
  }

  ~TreeRoot() = default;

  int columnCount() const
  {
    return 1;
  }

  QVariant data(int column, int role) const
  {
    if( column == 0 ) {
      if( role == Qt::DisplayRole ) {
        return QStringLiteral("File");
      }
    }
    return QVariant();
  }
};

class TreeFile : public TreeRoot {
public:
  TreeFile(const QString& filename, cs::AbstractTreeItem *parent = nullptr)
    : TreeRoot(parent)
    , _filename(filename)
  {
  }

  ~TreeFile() = default;

  QVariant data(int column, int role) const
  {
    if( column == 0 ) {
      if( role == Qt::DisplayRole ) {
        return _filename;
      }
    }
    return QVariant();
  }

private:
  QString _filename;
};

class TreeLine : public TreeRoot {
public:
  TreeLine(const QString& text, const int line,
           const QVector<int>& start, const QVector<int>& length,
           cs::AbstractTreeItem *parent = nullptr)
    : TreeRoot(parent)
    , _text(text)
    , _line(line)
    , _start(start)
    , _length(length)
  {
  }

  ~TreeLine() = default;

  QVariant data(int column, int role) const
  {
    using namespace QtCreator;
    if( column == 0 ) {
      if(        role == Qt::DisplayRole ) {
        return _text;
      } else if( role == int(HighlightingItemRole::LineNumber) ) {
        return _line;
      } else if( role == int(HighlightingItemRole::StartColumn) ) {
        return QVariant::fromValue(_start);
      } else if( role == int(HighlightingItemRole::Length) ) {
        return QVariant::fromValue(_length);
      } else if( role == int(HighlightingItemRole::Foreground) ) {
        return QColor(Qt::black);
      } else if( role == int(HighlightingItemRole::Background) ) {
        return QColor(Qt::yellow);
      }
    }
    return QVariant();
  }

private:
  QString _text;
  int     _line;
  QVector<int> _start;
  QVector<int> _length;
};

////// Create Model Data /////////////////////////////////////////////////////

cs::AbstractTreeItem *createModel()
{
  TreeRoot *root = new TreeRoot();

  {
    TreeFile *file1 = new TreeFile(QStringLiteral("File #1"));
    root->appendChild(file1);

    TreeLine *line11 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    1, {3, 26}, {3, 7});
    file1->appendChild(line11);

    TreeLine *line12 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    2, {7}, {5});
    file1->appendChild(line12);

    TreeLine *line13 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    3, {10}, {4});
    file1->appendChild(line13);
  }

  {
    TreeFile *file2 = new TreeFile(QStringLiteral("File #2"));
    root->appendChild(file2);

    TreeLine *line21 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    1, {3, 26}, {3, 7});
    file2->appendChild(line21);

    TreeLine *line22 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    2, {7}, {5});
    file2->appendChild(line22);

    TreeLine *line23 = new TreeLine(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                                    3, {10}, {4});
    file2->appendChild(line23);
  }

  return root;
}

////// Main //////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QTreeView *view = new QTreeView();
  view->header()->setVisible(true);
  view->setAlternatingRowColors(true);
  view->setEditTriggers(QAbstractItemView::NoEditTriggers);

  cs::TreeModel *model = new cs::TreeModel(createModel(), view);
  view->setModel(model);

  new ProxyDelegate(view);

  view->expandAll();
  view->header()->resizeSections(QHeaderView::ResizeToContents);
  view->setWindowTitle(QStringLiteral("Highlights"));
  view->setGeometry(100, 100, 640, 480);
  view->show();

  const int result = app.exec();
  delete view;

  return result;
}
