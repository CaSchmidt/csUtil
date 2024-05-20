/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <cs/Core/QStringUtil.h>

#include <Encode/Parser.h>

#include "Encoder/WEncoderPage.h"
#include "ui_WEncoderPage.h"

#include "Encoder/EncodeResultsModel.h"
#include "global.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl_encoder {

  struct EncoderPage {
    using Parser = Encode::Parser<uint64_t>;

    EncoderPage() noexcept = default;

    EncodeResultsModel *model{nullptr};
    Parser parser;
  };

  static_assert( std::is_same_v<EncoderPage::Parser::value_type,EncodeResultsModel::value_type> );

} // namespace impl_encoder

////// public ////////////////////////////////////////////////////////////////

WEncoderPage::WEncoderPage(QWidget *parent, const Qt::WindowFlags flags)
  : WTabPageBase(parent, flags)
  , ui(std::make_unique<Ui::WEncoderPage>())
  , d(new impl_encoder::EncoderPage)
{
  ui->setupUi(this);

  // Data Models /////////////////////////////////////////////////////////////

  d->model = new EncodeResultsModel(ui->resultsView);
  ui->resultsView->setModel(d->model);

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->clearButton, &QPushButton::clicked,
          this, &WEncoderPage::clearVariables);
  connect(ui->encodeButton, &QPushButton::clicked,
          this, &WEncoderPage::encode);
}

WEncoderPage::~WEncoderPage()
{
}

////// private slots /////////////////////////////////////////////////////////

void WEncoderPage::clearVariables()
{
}

void WEncoderPage::encode()
{
  using EnginePtr = impl_encoder::EncoderPage::Parser::EnginePtr;
  using     Store = impl_encoder::EncoderPage::Parser::Engine::Store;

  const std::string input = cs::toString(cs::toUtf8String(ui->editorWidget->toPlainText()));

  const bool ok = d->parser.parse(input, global::logger);
  if( !ok ) {
    return;
  }

  const Store store; // TODO

  d->model->clear();
  for(const EnginePtr& engine : d->parser.result) {
    d->model->addResult(engine, store, ui->msbFirstCheck->isChecked());
  }
}
