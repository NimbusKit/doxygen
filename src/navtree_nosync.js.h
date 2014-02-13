"var navTreeSubIndices = new Array();\n"
"\n"
"function getData(varName)\n"
"{\n"
"  var i = varName.lastIndexOf('/');\n"
"  var n = i>=0 ? varName.substring(i+1) : varName;\n"
"  return eval(n.replace(/\\-/g,'_'));\n"
"}\n"
"\n"
"function stripPath(uri)\n"
"{\n"
"  return uri.substring(uri.lastIndexOf('/')+1);\n"
"}\n"
"\n"
"function stripPath2(uri)\n"
"{\n"
"  var i = uri.lastIndexOf('/');\n"
"  var s = uri.substring(i+1);\n"
"  var m = uri.substring(0,i+1).match(/\\/d\\w\\/d\\w\\w\\/$/);\n"
"  return m ? uri.substring(i-6) : s;\n"
"}\n"
"\n"
"function localStorageSupported()\n"
"{\n"
"  try {\n"
"    return 'localStorage' in window && window['localStorage'] !== null && window.localStorage.getItem;\n"
"  }\n"
"  catch(e) {\n"
"    return false;\n"
"  }\n"
"}\n"
"\n"
"function getScript(scriptName,func,show)\n"
"{\n"
"  var head = document.getElementsByTagName(\"head\")[0]; \n"
"  var script = document.createElement('script');\n"
"  script.id = scriptName;\n"
"  script.type = 'text/javascript';\n"
"  script.onload = func; \n"
"  script.src = scriptName+'.js'; \n"
"  if ($.browser.msie && $.browser.version<=8) { \n"
"    // script.onload does not work with older versions of IE\n"
"    script.onreadystatechange = function() {\n"
"      if (script.readyState=='complete' || script.readyState=='loaded') { \n"
"        func(); if (show) showRoot(); \n"
"      }\n"
"    }\n"
"  }\n"
"  head.appendChild(script); \n"
"}\n"
"\n"
"function createIndent(o,domNode,node,level)\n"
"{\n"
"  var level=-1;\n"
"  var n = node;\n"
"  while (n.parentNode) { level++; n=n.parentNode; }\n"
"  if (node.childrenData) {\n"
"    var imgNode = document.createElement(\"img\");\n"
"    imgNode.style.paddingLeft=(16*level).toString()+'px';\n"
"    imgNode.width  = 16;\n"
"    imgNode.height = 22;\n"
"    imgNode.border = 0;\n"
"    node.plus_img = imgNode;\n"
"    node.expandToggle = document.createElement(\"a\");\n"
"    node.expandToggle.href = \"javascript:void(0)\";\n"
"    node.expandToggle.onclick = function() {\n"
"      if (node.expanded) {\n"
"        $(node.getChildrenUL()).slideUp(\"fast\");\n"
"        node.plus_img.src = node.relpath+\"ftv2pnode.png\";\n"
"        node.expanded = false;\n"
"      } else {\n"
"        expandNode(o, node, false, false);\n"
"      }\n"
"    }\n"
"    node.expandToggle.appendChild(imgNode);\n"
"    domNode.appendChild(node.expandToggle);\n"
"    imgNode.src = node.relpath+\"ftv2pnode.png\";\n"
"  } else {\n"
"    var span = document.createElement(\"span\");\n"
"    span.style.display = 'inline-block';\n"
"    span.style.width   = 16*(level+1)+'px';\n"
"    span.style.height  = '22px';\n"
"    span.innerHTML = '&#160;';\n"
"    domNode.appendChild(span);\n"
"  } \n"
"}\n"
"\n"
"var animationInProgress = false;\n"
"\n"
"function gotoAnchor(anchor,aname,updateLocation)\n"
"{\n"
"  var pos, docContent = $('#doc-content');\n"
"  if (anchor.parent().attr('class')=='memItemLeft' ||\n"
"      anchor.parent().attr('class')=='fieldtype' ||\n"
"      anchor.parent().is(':header')) \n"
"  {\n"
"    pos = anchor.parent().position().top;\n"
"  } else if (anchor.position()) {\n"
"    pos = anchor.position().top;\n"
"  }\n"
"  if (pos) {\n"
"    var dist = Math.abs(Math.min(\n"
"               pos-docContent.offset().top,\n"
"               docContent[0].scrollHeight-\n"
"               docContent.height()-docContent.scrollTop()));\n"
"    animationInProgress=true;\n"
"    docContent.animate({\n"
"      scrollTop: pos + docContent.scrollTop() - docContent.offset().top\n"
"    },Math.max(50,Math.min(500,dist)),function(){\n"
"      if (updateLocation) window.location.href=aname;\n"
"      animationInProgress=false;\n"
"    });\n"
"  }\n"
"}\n"
"\n"
"function newNode(o, po, text, link, childrenData, lastNode)\n"
"{\n"
"  var node = new Object();\n"
"  node.children = Array();\n"
"  node.childrenData = childrenData;\n"
"  node.depth = po.depth + 1;\n"
"  node.relpath = po.relpath;\n"
"  node.isLast = lastNode;\n"
"\n"
"  node.li = document.createElement(\"li\");\n"
"  po.getChildrenUL().appendChild(node.li);\n"
"  node.parentNode = po;\n"
"\n"
"  node.itemDiv = document.createElement(\"div\");\n"
"  node.itemDiv.className = \"item\";\n"
"\n"
"  node.labelSpan = document.createElement(\"span\");\n"
"  node.labelSpan.className = \"label\";\n"
"\n"
"  createIndent(o,node.itemDiv,node,0);\n"
"  node.itemDiv.appendChild(node.labelSpan);\n"
"  node.li.appendChild(node.itemDiv);\n"
"\n"
"  var a = document.createElement(\"a\");\n"
"  node.labelSpan.appendChild(a);\n"
"  node.label = document.createTextNode(text);\n"
"  node.expanded = false;\n"
"  a.appendChild(node.label);\n"
"  if (link) {\n"
"    var url;\n"
"    if (link.substring(0,1)=='^') {\n"
"      url = link.substring(1);\n"
"      link = url;\n"
"    } else {\n"
"      url = node.relpath+link;\n"
"    }\n"
"    a.className = stripPath(link.replace('#',':'));\n"
"    if (link.indexOf('#')!=-1) {\n"
"      var aname = '#'+link.split('#')[1];\n"
"      var srcPage = stripPath($(location).attr('pathname'));\n"
"      var targetPage = stripPath(link.split('#')[0]);\n"
"      a.href = srcPage!=targetPage ? url : \"javascript:void(0)\"; \n"
"      a.onclick = function(){\n"
"        storeLink(link);\n"
"        if (!$(a).parent().parent().hasClass('selected'))\n"
"        {\n"
"          $('.item').removeClass('selected');\n"
"          $('.item').removeAttr('id');\n"
"          $(a).parent().parent().addClass('selected');\n"
"          $(a).parent().parent().attr('id','selected');\n"
"        }\n"
"        var anchor = $(aname);\n"
"        gotoAnchor(anchor,aname,true);\n"
"      };\n"
"    } else {\n"
"      a.href = url;\n"
"      a.onclick = function() { storeLink(link); }\n"
"    }\n"
"  } else {\n"
"    if (childrenData != null) \n"
"    {\n"
"      a.className = \"nolink\";\n"
"      a.href = \"javascript:void(0)\";\n"
"      a.onclick = node.expandToggle.onclick;\n"
"    }\n"
"  }\n"
"\n"
"  node.childrenUL = null;\n"
"  node.getChildrenUL = function() {\n"
"    if (!node.childrenUL) {\n"
"      node.childrenUL = document.createElement(\"ul\");\n"
"      node.childrenUL.className = \"children_ul\";\n"
"      node.childrenUL.style.display = \"none\";\n"
"      node.li.appendChild(node.childrenUL);\n"
"    }\n"
"    return node.childrenUL;\n"
"  };\n"
"\n"
"  return node;\n"
"}\n"
"\n"
"function showRoot()\n"
"{\n"
"  var headerHeight = $(\"#top\").height();\n"
"  var footerHeight = $(\"#nav-path\").height();\n"
"  var windowHeight = $(window).height() - headerHeight - footerHeight;\n"
"  (function (){ // retry until we can scroll to the selected item\n"
"    try {\n"
"      var navtree=$('#nav-tree');\n"
"      navtree.scrollTo('#selected',0,{offset:-windowHeight/2});\n"
"    } catch (err) {\n"
"      setTimeout(arguments.callee, 0);\n"
"    }\n"
"  })();\n"
"}\n"
"\n"
"function expandNode(o, node, imm, showRoot)\n"
"{\n"
"  if (node.childrenData && !node.expanded) {\n"
"    if (typeof(node.childrenData)==='string') {\n"
"      var varName    = node.childrenData;\n"
"      getScript(node.relpath+varName,function(){\n"
"        node.childrenData = getData(varName);\n"
"        expandNode(o, node, imm, showRoot);\n"
"      }, showRoot);\n"
"    } else {\n"
"      if (!node.childrenVisited) {\n"
"        getNode(o, node);\n"
"      } if (imm || ($.browser.msie && $.browser.version>8)) { \n"
"        // somehow slideDown jumps to the start of tree for IE9 :-(\n"
"        $(node.getChildrenUL()).show();\n"
"      } else {\n"
"        $(node.getChildrenUL()).slideDown(\"fast\");\n"
"      }\n"
"      if (node.isLast) {\n"
"        node.plus_img.src = node.relpath+\"ftv2mlastnode.png\";\n"
"      } else {\n"
"        node.plus_img.src = node.relpath+\"ftv2mnode.png\";\n"
"      }\n"
"      node.expanded = true;\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"function glowEffect(n,duration)\n"
"{\n"
"  n.addClass('glow').delay(duration).queue(function(next){\n"
"    $(this).removeClass('glow');next();\n"
"  });\n"
"}\n"
"\n"
"function highlightAnchor()\n"
"{\n"
"  var aname = $(location).attr('hash');\n"
"  var anchor = $(aname);\n"
"  if (anchor.parent().attr('class')=='memItemLeft'){\n"
"    var rows = $('.memberdecls tr[class$=\"'+\n"
"               window.location.hash.substring(1)+'\"]');\n"
"    glowEffect(rows.children(),300); // member without details\n"
"  } else if (anchor.parents().slice(2).prop('tagName')=='TR') {\n"
"    glowEffect(anchor.parents('div.memitem'),1000); // enum value\n"
"  } else if (anchor.parent().attr('class')=='fieldtype'){\n"
"    glowEffect(anchor.parent().parent(),1000); // struct field\n"
"  } else if (anchor.parent().is(\":header\")) {\n"
"    glowEffect(anchor.parent(),1000); // section header\n"
"  } else {\n"
"    glowEffect(anchor.next(),1000); // normal member\n"
"  }\n"
"  gotoAnchor(anchor,aname,false);\n"
"}\n"
"\n"
"function selectAndHighlight(hash,n)\n"
"{\n"
"  var a;\n"
"  if (hash) {\n"
"    var link=stripPath($(location).attr('pathname'))+':'+hash.substring(1);\n"
"    a=$('.item a[class$=\"'+link+'\"]');\n"
"  }\n"
"  if (a && a.length) {\n"
"    a.parent().parent().addClass('selected');\n"
"    a.parent().parent().attr('id','selected');\n"
"    highlightAnchor();\n"
"  } else if (n) {\n"
"    $(n.itemDiv).addClass('selected');\n"
"    $(n.itemDiv).attr('id','selected');\n"
"  }\n"
"  showRoot();\n"
"}\n"
"\n"
"function showNode(o, node, index, hash)\n"
"{\n"
"  if (node && node.childrenData) {\n"
"    if (typeof(node.childrenData)==='string') {\n"
"      var varName    = node.childrenData;\n"
"      getScript(node.relpath+varName,function(){\n"
"        node.childrenData = getData(varName);\n"
"        showNode(o,node,index,hash);\n"
"      },true);\n"
"    } else {\n"
"      if (!node.childrenVisited) {\n"
"        getNode(o, node);\n"
"      }\n"
"      $(node.getChildrenUL()).css({'display':'block'});\n"
"      if (node.isLast) {\n"
"        node.plus_img.src = node.relpath+\"ftv2mlastnode.png\";\n"
"      } else {\n"
"        node.plus_img.src = node.relpath+\"ftv2mnode.png\";\n"
"      }\n"
"      node.expanded = true;\n"
"      var n = node.children[o.breadcrumbs[index]];\n"
"      if (index+1<o.breadcrumbs.length) {\n"
"        showNode(o,n,index+1,hash);\n"
"      } else {\n"
"        if (typeof(n.childrenData)==='string') {\n"
"          var varName = n.childrenData;\n"
"          getScript(n.relpath+varName,function(){\n"
"            n.childrenData = getData(varName);\n"
"            node.expanded=false;\n"
"            showNode(o,node,index,hash); // retry with child node expanded\n"
"          },true);\n"
"        } else {\n"
"          var rootBase = stripPath(o.toroot.replace(/\\..+$/, ''));\n"
"          if (rootBase==\"index\" || rootBase==\"pages\" || rootBase==\"search\") {\n"
"            expandNode(o, n, true, true);\n"
"          }\n"
"          selectAndHighlight(hash,n);\n"
"        }\n"
"      }\n"
"    }\n"
"  } else {\n"
"    selectAndHighlight(hash);\n"
"  }\n"
"}\n"
"\n"
"function removeToInsertLater(element) {\n"
"  var parentNode = element.parentNode;\n"
"  var nextSibling = element.nextSibling;\n"
"  parentNode.removeChild(element);\n"
"  return function() {\n"
"    if (nextSibling) {\n"
"      parentNode.insertBefore(element, nextSibling);\n"
"    } else {\n"
"      parentNode.appendChild(element);\n"
"    }\n"
"  };\n"
"}\n"
"\n"
"function getNode(o, po)\n"
"{\n"
"  var insertFunction = removeToInsertLater(po.li);\n"
"  po.childrenVisited = true;\n"
"  var l = po.childrenData.length-1;\n"
"  for (var i in po.childrenData) {\n"
"    var nodeData = po.childrenData[i];\n"
"    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],\n"
"      i==l);\n"
"  }\n"
"  insertFunction();\n"
"}\n"
"\n"
"function gotoNode(o,subIndex,root,hash,relpath)\n"
"{\n"
"  var nti = navTreeSubIndices[subIndex][root+hash];\n"
"  o.breadcrumbs = $.extend(true, [], nti ? nti : navTreeSubIndices[subIndex][root]);\n"
"  if (!o.breadcrumbs && root!=NAVTREE[0][1]) { // fallback: show index\n"
"    navTo(o,NAVTREE[0][1],\"\",relpath);\n"
"    $('.item').removeClass('selected');\n"
"    $('.item').removeAttr('id');\n"
"  }\n"
"  if (o.breadcrumbs) {\n"
"    if (o.breadcrumbs.length == 0) {\n"
"      o.breadcrumbs.unshift(0); // add 0 for root node\n"
"    }\n"
"    showNode(o, o.node, 0, hash);\n"
"  }\n"
"}\n"
"\n"
"function navTo(o,root,hash,relpath)\n"
"{\n"
"  if (hash.match(/^#l\\d+$/)) {\n"
"    var anchor=$('a[name='+hash.substring(1)+']');\n"
"    glowEffect(anchor.parent(),1000); // line number\n"
"    hash=''; // strip line number anchors\n"
"    //root=root.replace(/_source\\./,'.'); // source link to doc link\n"
"  }\n"
"  var url=root+hash;\n"
"  var i=-1;\n"
"  while (NAVTREEINDEX[i+1]<=url) i++;\n"
"  if (i==-1) { i=0; root=NAVTREE[0][1]; } // fallback: show index\n"
"  if (navTreeSubIndices[i]) {\n"
"    gotoNode(o,i,root,hash,relpath)\n"
"  } else {\n"
"    getScript(relpath+'navtreeindex'+i,function(){\n"
"      navTreeSubIndices[i] = eval('NAVTREEINDEX'+i);\n"
"      if (navTreeSubIndices[i]) {\n"
"        gotoNode(o,i,root,hash,relpath);\n"
"      }\n"
"    },true);\n"
"  }\n"
"}\n"
"\n"
"function initNavTree(toroot,relpath)\n"
"{\n"
"  var o = new Object();\n"
"  o.toroot = toroot;\n"
"  o.node = new Object();\n"
"  o.node.li = document.getElementById(\"nav-tree-contents\");\n"
"  o.node.childrenData = NAVTREE;\n"
"  o.node.children = new Array();\n"
"  o.node.childrenUL = document.createElement(\"ul\");\n"
"  o.node.getChildrenUL = function() { return o.node.childrenUL; };\n"
"  o.node.li.appendChild(o.node.childrenUL);\n"
"  o.node.depth = 0;\n"
"  o.node.relpath = relpath;\n"
"  o.node.expanded = false;\n"
"  o.node.isLast = true;\n"
"  o.node.plus_img = document.createElement(\"img\");\n"
"  o.node.plus_img.src = relpath+\"ftv2pnode.png\";\n"
"  o.node.plus_img.width = 16;\n"
"  o.node.plus_img.height = 22;\n"
"\n"
"  $(window).load(function(){\n"
"    navTo(o,toroot,window.location.hash,relpath);\n"
"    showRoot();\n"
"  });\n"
"\n"
"  $(window).bind('hashchange', function(){\n"
"     if (window.location.hash && window.location.hash.length>1){\n"
"       var a;\n"
"       if ($(location).attr('hash')){\n"
"         var clslink=stripPath($(location).attr('pathname'))+':'+\n"
"                               $(location).attr('hash').substring(1);\n"
"         a=$('.item a[class$=\"'+clslink+'\"]');\n"
"       }\n"
"       if (a==null || !$(a).parent().parent().hasClass('selected')){\n"
"         $('.item').removeClass('selected');\n"
"         $('.item').removeAttr('id');\n"
"       }\n"
"       var link=stripPath2($(location).attr('pathname'));\n"
"       navTo(o,link,$(location).attr('hash'),relpath);\n"
"     } else if (!animationInProgress) {\n"
"       $('#doc-content').scrollTop(0);\n"
"       $('.item').removeClass('selected');\n"
"       $('.item').removeAttr('id');\n"
"       navTo(o,toroot,window.location.hash,relpath);\n"
"     }\n"
"  })\n"
"}\n"
"\n"
"\n"