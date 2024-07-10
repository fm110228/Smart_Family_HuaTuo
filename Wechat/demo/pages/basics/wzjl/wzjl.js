const app = getApp();
Page({
  data: {
    StatusBar: app.globalData.StatusBar,
    CustomBar: app.globalData.CustomBar,
    index: null,
    picker: ['喵喵喵', '汪汪汪', '哼唧哼唧'],
    multiArray: [
      ['血常规', '肝胆','心脏'],
      ['血常规', '血常规', '血常规'],
      ['肝胆', '肝胆','肝胆']
    ],
    objectMultiArray: [
      [{
          id: 0,
          name: '血常规'
        },
        {
          id: 1,
          name: '血常规'
        }
      ],
      [{
          id: 0,
          name: '血常规'
        },
        {
          id: 1,
          name: '血常规'
        },
        {
          id: 2,
          name: '血常规'
        },
        {
          id: 3,
          name: '血常规'
        },
        {
          id: 3,
          name: '血常规'
        }
      ],
      [{
          id: 0,
          name: '血常规'
        },
        {
          id: 1,
          name: '血常规'
        }
      ]
    ],
    multiIndex: [0, 0, 0],
    time: '12:01',
    date: '2018-12-25',
    region: ['广东省', '广州市', '海珠区'],
    imgList: [],
    modalName: null,
    textareaAValue: '',
    textareaBValue: ''
  },
  PickerChange(e) {
    console.log(e);
    this.setData({
      index: e.detail.value
    })
  },
  MultiChange(e) {
    this.setData({
      multiIndex: e.detail.value
    })
  },
  MultiColumnChange(e) {
    let data = {
      multiArray: this.data.multiArray,
      multiIndex: this.data.multiIndex
    };
    data.multiIndex[e.detail.column] = e.detail.value;
    switch (e.detail.column) {
      case 0:
        switch (data.multiIndex[0]) {
          case 0:
            data.multiArray[1] = ['血常规', '血常规', '血常规', '血常规', '血常规'];
            data.multiArray[2] = ['血常规', '血常规'];
            break;
          case 1:
            data.multiArray[1] = ['血常规', '血常规', '血常规'];
            data.multiArray[2] = ['血常规', '血常规'];
            break;
        }
        data.multiIndex[1] = 0;
        data.multiIndex[2] = 0;
        break;
      case 1:
        switch (data.multiIndex[0]) {
          case 0:
            switch (data.multiIndex[1]) {
              case 0:
                data.multiArray[2] = ['血常规', '血常规'];
                break;
              case 1:
                data.multiArray[2] = ['血常规'];
                break;
              case 2:
                data.multiArray[2] = ['血常规', '血常规'];
                break;
              case 3:
                data.multiArray[2] = ['血常规', '血常规', '血常规'];
                break;
              case 4:
                data.multiArray[2] = ['血常规', '血常规', '血常规', '血常规'];
                break;
            }
            break;
          case 1:
            switch (data.multiIndex[1]) {
              case 0:
                data.multiArray[2] = ['血常规', '血常规'];
                break;
              case 1:
                data.multiArray[2] = ['血常规', '血常规'];
                break;
              case 2:
                data.multiArray[2] = ['血常规', '血常规', '血常规'];
                break;
            }
            break;
        }
        data.multiIndex[2] = 0;
        break;
    }
    this.setData(data);
  },
  TimeChange(e) {
    this.setData({
      time: e.detail.value
    })
  },
  DateChange(e) {
    this.setData({
      date: e.detail.value
    })
  },
  RegionChange: function(e) {
    this.setData({
      region: e.detail.value
    })
  },
  ChooseImage() {
    wx.chooseImage({
      count: 4, //默认9
      sizeType: ['original', 'compressed'], //可以指定是原图还是压缩图，默认二者都有
      sourceType: ['album'], //从相册选择
      success: (res) => {
        if (this.data.imgList.length != 0) {
          this.setData({
            imgList: this.data.imgList.concat(res.tempFilePaths)
          })
        } else {
          this.setData({
            imgList: res.tempFilePaths
          })
        }
      }
    });
  },
  ViewImage(e) {
    wx.previewImage({
      urls: this.data.imgList,
      current: e.currentTarget.dataset.url
    });
  },
  DelImg(e) {
    wx.showModal({
      title: '召唤师',
      content: '确定要删除这段回忆吗？',
      cancelText: '再看看',
      confirmText: '再见',
      success: res => {
        if (res.confirm) {
          this.data.imgList.splice(e.currentTarget.dataset.index, 1);
          this.setData({
            imgList: this.data.imgList
          })
        }
      }
    })
  },
  textareaAInput(e) {
    this.setData({
      textareaAValue: e.detail.value
    })
  },
  textareaBInput(e) {
    this.setData({
      textareaBValue: e.detail.value
    })
  }
})