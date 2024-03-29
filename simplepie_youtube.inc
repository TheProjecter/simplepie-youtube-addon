<?php
/**
 * SimplePie Add-on for YouTube
 *
 * Copyright (c) 2004-2007, Peet Sneekes
 * All rights reserved. License matches the current SimplePie license.
 */
 
if (!defined('SIMPLEPIE_NAMESPACE_YOUTUBE'))
{
	define('SIMPLEPIE_NAMESPACE_YOUTUBE', 'http://www.w3.org/2005/Atom');
	define('SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA', 'http://search.yahoo.com/mrss/');
	define('SIMPLEPIE_NAMESPACE_YOUTUBE_APP', 'http://purl.org/atom/app#');
	define('SIMPLEPIE_NAMESPACE_YOUTUBE_OPENSEARCH', 'http://a9.com/-/spec/opensearchrss/1.0/');
	define('SIMPLEPIE_NAMESPACE_YOUTUBE_YT', 'http://gdata.youtube.com/schemas/2007');
	define('SIMPLEPIE_NAMESPACE_YOUTUBE_GD', 'http://schemas.google.com/g/2005');	
}
class SimplePie_Item_YouTube extends SimplePie_Item
{
	/*
	 * @var bool is this a youtube item?
	 * @access private
	 */
	var $is_youtube = true;
	// new function
	function get_youtube_videoid()
	{
		$data = $this->get_youtube_player_url();
		// FIX ME : check and if possible use reg exp to extract the video id.
		$data = substr($data,strpos($data,"/v/")+3,strpos($data,"/v/") - strpos($data,"feature")-7);
		return $data;
	}
 	// New method
	function get_youtube_player()
	{
	    $data = "<object width='480' height='385'>\n";
	    $data .= "<param name='movie' value='" . $this->get_youtube_player_url() . "&hl=en_US&fs=1&rel=0'></param>\n";
	    $data .= "<param name='allowFullScreen' value='true'></param>\n";
	    $data .= "<param name='allowscriptaccess' value='always'></param>\n";
	    $data .= "<embed src='" . $this->get_youtube_player_url() . "&hl=en_US&fs=1&rel=0' type='application/x-shockwave-flash' allowscriptaccess='always' allowfullscreen='true' width='480' height='385'>\n</embed>";
	    $data .= "</object><br />";
	    return $data;
	}
	function get_youtube_player_url()
	{

		// checking the playlist style url
		$data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA, 'group');
		$data = $data[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA]['content'];
		$data = $data[0]['attribs']['']['url'];
		if ($data == null )
		{
			//playlist not found then looking if its a tag style url placement
			$data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE, 'link');
			$data =  $data[0]['attribs']['']['href'];
			$data = str_replace('/watch?v=', '/v/', $data);
			return $data;
		}
		return $data;
	}
 
	// New method
	function get_youtube_thumbnail_url($no = 0)
	{
	    $data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA, 'group');
		$data = $data[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA]['thumbnail'];
		$data = $data[$no]['attribs']['']['url'];
		if ($data == null)
		{
			$data =  "http://img.youtube.com/vi/" . $this->get_youtube_videoid() . "/" . $no .  ".jpg";
		}
		return $data;
	}
 
	// New method
	function get_youtube_thumbnail_width($no = 0)
	{
	    $data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA, 'group');
		$data = $data[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA]['thumbnail'];
		return $data[$no]['attribs']['']['width'];
	}
 
	// New method
	function get_youtube_thumbnail_height($no = 0)
	{
	    $data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA, 'group');
		$data = $data[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA]['thumbnail'];
		return $data[$no]['attribs']['']['height'];
	}
 
	// New method
	function get_youtube_title()
	{
	if (!$this->is_fav)
	{
		$data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE, 'title');
		return $data[0]['data'];
	}
	}
 
	// New method
	function get_youtube_author()
	{
		$data = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE, 'author');
		$data = $data[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE]['name'];
		return $data[0]['data'];
	}
	// overloading the parent functions
    function get_content()
    {
        $data = $this->get_youtube_player();
		return $data;
    } 
	// Overloading the categories method
	function get_categories()
	{
		$categories = array();
        $cat_list = $this->get_item_tags(SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA, 'group');
        $cat_list = $cat_list[0]['child'][SIMPLEPIE_NAMESPACE_YOUTUBE_MEDIA]['category'];
		foreach ((array) $cat_list as $category)
		{
			$term = null;
			$scheme = null;
			$label = null;
			if (isset($category['attribs']['']['term']))
			{
				$term = $this->sanitize($category['attribs']['']['term'], SIMPLEPIE_CONSTRUCT_TEXT);
				echo $term;
			}
			if (isset($category['attribs']['']['scheme']))
			{
				$scheme = $this->sanitize($category['attribs']['']['scheme'], SIMPLEPIE_CONSTRUCT_TEXT);
			}
			if (isset($category['attribs']['']['label']))
			{
				$label = $this->sanitize($category['attribs']['']['label'], SIMPLEPIE_CONSTRUCT_TEXT);
			}
			$categories[] =& new $this->feed->category_class($term, $scheme, $label);
		}
		if (!empty($categories))
		{
			return SimplePie_Misc::array_unique($categories);
		}
		else
		{
			return null;
		}
	}
}
?>
