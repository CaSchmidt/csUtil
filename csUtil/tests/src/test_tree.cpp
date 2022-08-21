#include <cstdio>
#include <cstdlib>

#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>

#include <cs/Qt/AbstractTreeItem.h>
#include <cs/Qt/ItemModel.h>
#include <cs/Qt/TreeModel.h>

void print_depth(const QAbstractItemModel *model,
                 const int row, const QModelIndex& parent = QModelIndex())
{
  const QModelIndex index = model->index(row, 0, parent);
  const QString text = model->data(index).toString();
  printf("row = %d, depth = %d, text = %s\n",
         index.row(), cs::indexDepth(index), qPrintable(text));
  fflush(stdout);
}

class TreeItem : public cs::AbstractTreeItem {
public:
  TreeItem(cs::AbstractTreeItem *parent = nullptr)
    : cs::AbstractTreeItem(parent)
  {
  }

  ~TreeItem()
  {
  }

  int columnCount() const
  {
    return 2;
  }

  QVariant data(int column, int role) const
  {
    if( role == Qt::DisplayRole ) {
      if(        column == 0 ) {
        return QString(QStringLiteral("Key"));
      } else if( column == 1 ) {
        return QString(QStringLiteral("Value"));
      }
    }
    return QVariant();
  }
};

class TreeValue : public TreeItem {
public:
  TreeValue(const QString& key, const QVariant& value,
            TreeItem *parent = nullptr)
    : TreeItem(parent)
    , _key(key)
    , _value(value)
  {
  }

  ~TreeValue()
  {
  }

  QVariant data(int column, int role) const
  {
    if( role == Qt::DisplayRole ) {
      if(        column == 0 ) {
        return _key;
      } else if( column == 1 ) {
        return _value;
      }
    }
    return QVariant();
  }

private:
  QString _key;
  QVariant _value;
};

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QTreeView *view = new QTreeView();
  view->header()->setVisible(true);
  view->setAlternatingRowColors(true);
  view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  view->setSelectionBehavior(QAbstractItemView::SelectRows);
  view->setSelectionMode(QAbstractItemView::SingleSelection);
  view->setWindowTitle(QStringLiteral("Tree"));
  view->show();

  // Begin Data //////////////////////////////////////////////////////////////

  TreeItem *root = new TreeItem();

  TreeValue *child1 = new TreeValue(QStringLiteral("child 1"), QStringLiteral("one"));
  root->appendChild(child1);

  TreeValue *child11 = new TreeValue(QStringLiteral("child 1.1"), 1);
  child1->appendChild(child11);
  TreeValue *child12 = new TreeValue(QStringLiteral("child 1.2"), 2);
  child1->appendChild(child12);
  TreeValue *child13 = new TreeValue(QStringLiteral("child 1.3"), 3);
  child1->appendChild(child13);

  TreeValue *child2 = new TreeValue(QStringLiteral("child 2"), QStringLiteral("two"));
  root->appendChild(child2);

  TreeValue *child21 = new TreeValue(QStringLiteral("child 2.1"), 0.1);
  child2->appendChild(child21);
  TreeValue *child22 = new TreeValue(QStringLiteral("child 2.2"), 0.2);
  child2->appendChild(child22);

  TreeValue *child3 = new TreeValue(QStringLiteral("child 3"), QStringLiteral("three"));
  root->appendChild(child3);

  // End Data ////////////////////////////////////////////////////////////////

#if 0
  cs::TreeModel *model = new cs::TreeModel(root, view);
#else
  cs::TreeModel *model = new cs::TreeModel(nullptr, view);
  model->setRoot(root);
#endif
  view->setModel(model);
  view->expandAll();
  view->header()->resizeSections(QHeaderView::ResizeToContents);

  print_depth(model, 1);
  print_depth(model, 1, model->index(1, 0));

  const int result = app.exec();
  delete view;

  return result;
}
